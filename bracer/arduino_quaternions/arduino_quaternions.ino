#include <SPI.h>
#include <Wire.h>
#include "SparkFunMPU9250-DMP.h"

MPU9250_DMP imu;

float currx  = 0;
float lastx1 = 0;
float lastx2 = 0;
float curry  = 0;
float lasty1 = 0;
float lasty2 = 0;
float currz  = 0;
float lastz1 = 0;
float lastz2 = 0;

float dt = 0.0f;
float last_time_dt;
unsigned long last_time_interval;
unsigned long interval = 100000.0f;

float x = 0.0f;
float y = 0.0f;
float z = 0.0f;

void setup() {
  Wire.begin();
  Serial.begin(9600); // Debug serial.
  Serial3.begin(115200); // Serial to ESP8266WiFi.

  /**
   * BEGIN: I2C Scanner.
   */

  while (!Serial);
  Serial.println("I2C Scanner");

  byte error, address;
  int nDevices;

  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 8; address < 127; address++ ) {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0) {
          Serial.print("I2C device found at address 0x");
          if (address < 16) {
            Serial.print("0");
          }
          Serial.print(address, HEX);
          Serial.println(" !");

          nDevices++;
      } else if (error == 4) {
          Serial.print("Unknow error at address 0x");
          if (address < 16) {
            Serial.print("0");
          }
          Serial.println(address, HEX);
      } 
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.println("Done!");
  }

  delay(1000);

  /**
   * END: I2C Scanner.
   */

  if (imu.begin() != INV_SUCCESS) {
    while (true) {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(12000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);

  while (!imu.fifoAvailable() && !imu.dmpUpdateFifo() == INV_SUCCESS);  
  delay(10000);
  
  imu.updateAccel();
  lastx2 = imu.calcAccel(imu.ax);
  lasty2 = imu.calcAccel(imu.ay);
  lastz2 = imu.calcAccel(imu.az);

  imu.updateAccel();
  lastx1 = imu.calcAccel(imu.ax);
  lasty1 = imu.calcAccel(imu.ay);
  lastz1 = imu.calcAccel(imu.az);
  
  for (int i = 0; i < 30; ++i) {
      imu.dmpUpdateFifo();
      updateData(1.0f);
      clearData();
  }

  last_time_dt = micros();
  last_time_interval = micros();

  Serial.println("Start loop");
}

void loop() {
  dt = (micros() - last_time_dt) / 1000000.0f;
  last_time_dt = micros();

  updateData(dt);

  if (micros() - last_time_interval > interval) {
    last_time_interval = micros();

    String data = getData();
    Serial.println(data);
    Serial3.println(data);

    clearData();
  }
}

//void set_in_range(float &value, float new_value, float from, float to) {
//  if (value >= from && value <= to) {
//    value = new_value;
//  }
//}
 
String getData() {
  String data = "";

  data += String(x, 6) + ' ';
  data += String(y, 6) + ' ';
  data += String(z, 6);

  return data;
}

void quat_mult(const float *q1, const float *q2, float *result) {
    float q1w = q1[0];
    float q1x = q1[1];
    float q1y = q1[2];
    float q1z = q1[3];

    float q2w = q2[0];
    float q2x = q2[1];
    float q2y = q2[2];
    float q2z = q2[3];

    result[0] = q1w * q2w - q1x * q2x - q1y * q2y - q1z * q2z;
    result[1] = q1w * q2x + q1x * q2w + q1y * q2z - q1z * q2y;
    result[2] = q1w * q2y - q1x * q2z + q1y * q2w + q1z * q2x;
    result[3] = q1w * q2z + q1x * q2y - q1y * q2x + q1z * q2w;
}

void quat_conj(const float *q, float *result) {
  result[0] = q[0];
  result[1] = -q[1];
  result[2] = -q[2];
  result[3] = -q[3];
}

void updateData(float dt) {
//  if (!imu.fifoAvailable() ||
//      imu.dmpUpdateFifo() != INV_SUCCESS)
//  {
//    return;
//  }
  imu.update(UPDATE_ACCEL | UPDATE_GYRO);

//  imu.computeEulerAngles();
  float qw = imu.calcQuat(imu.qw);
  float qx = imu.calcQuat(imu.qx);
  float qy = imu.calcQuat(imu.qy);
  float qz = imu.calcQuat(imu.qz);

  lastx2 = lastx1;
  lasty2 = lasty1;
  lastz2 = lastz1;

  lastx1 = currx;
  lasty1 = curry;
  lastz1 = currz;

//  imu.updateAccel();
  currx = imu.calcAccel(imu.ax);
  curry = imu.calcAccel(imu.ay);
  currz = imu.calcAccel(imu.az);

  float ax = (currx + lastx1 + lastx2) / 3;
  float ay = (curry + lasty1 + lasty2) / 3;
  float az = (currz + lastz1 + lastz2) / 3;

  Serial.println("Q: " + String(qw, 6) + ' ' + String(qx, 6) + ' ' + String(qy, 6) + ' ' + String(qz, 6));
  Serial.println("A: " + String(ax, 6) + ' ' + String(ay, 6) + ' ' + String(az, 6));

  // Quaternion:
  float q[4] = { qw, qx, qy, qz };

  // Quaternion on vector:
  float qv[4] = { 0.0f, ax, ay, az };
  float qrv[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  quat_mult(q, qv, qrv);

  // Last on conjugate:
  float qc[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  quat_conj(q, qc);
  float qrc[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  quat_mult(qrv, qc, qrc);

  float ar[3] = { qrc[1] * dt, qrc[2] * dt, (qrc[3] - 1.0f) * dt };

  // float cl = 0.001f;
  // float va = 0.0f;
  // set_in_range(ar[0], va, cl, -cl);
  // set_in_range(ar[1], va, cl, -cl);
  // set_in_range(ar[2], va, cl, -cl);

  Serial.println("AR: " + String(ar[0], 6) + ' ' + String(ar[1], 6) + ' ' + String(ar[2], 6));

  x += ar[0];
  y += ar[1];
  z += ar[2];
}

void clearData() {
  x = 0;
  y = 0;
  z = 0;
}
