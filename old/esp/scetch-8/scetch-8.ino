#include <SPI.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include "SparkFunMPU9250-DMP.h"

#define K 0.2
#define SERVER_PORT 7247
#define SERVER_IP IPAddress(10, 42, 0, 1)

WiFiClient client;
MPU9250_DMP imu;

const char *SSID = "MS-7971";
const char *PASSWORD = "UfnbalFT";

float dt = 0.0f;
float last_time_dt;
unsigned long last_time_interval;
unsigned long interval = 100000.0f;

float optX;
float optY;
float optZ;

float optX_last;
float optY_last;
float optZ_last;

float quat[4];

void I2C_scanner();
void start_mpu();
void kalman_filter();
float clamp(float value, float min_value, float max_value);
String getData();
void quat_mult(const float *q1, const float *q2, float *result);
void quat_conj(const float *q, float *result);
void updateData(float dt);
void connect(const char *SSID, const char *PASSWORD);

//////////////////////////////////////////////////////////////////
/////////////////// Main functions ///////////////////////////////
//////////////////////////////////////////////////////////////////

void setup() {
  Wire.begin();
  Serial.begin(115200);

  Serial.println("Setup");

  I2C_scanner();
  start_mpu();

  Serial.println("Start loop");
}

void loop() {
  Serial.println("Loop");
}

//////////////////////////////////////////////////////////////////
/////////////////// End main functions ///////////////////////////
//////////////////////////////////////////////////////////////////

float clamp(float value, float min_value, float max_value) {
  return min(max(value, max_value), min_value);
}

void I2C_scanner() {
  while (!Serial);
  Serial.println("\nI2C Scanner");

  byte error, address;
  int nDevices, count_iter = 0;;

  Serial.println("Scanning...");
  while (1) {
    nDevices = 0;
    for (address = 8; address < 127; address++ ) {
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
    ++count_iter;
    if (nDevices) {
      Serial.println("done\n");
      break;
    } else {
      Serial.println("No i2c devices found\n");
      if (count_iter > 5) {
        Serial.println("Please, reboot device");
      }
    }
  }
}

void start_mpu() {
  //connect(SSID, PASSWORD);
  if (imu.begin() != INV_SUCCESS) {
    while (1) {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(12000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);

  while (!imu.fifoAvailable() && !imu.dmpUpdateFifo() == INV_SUCCESS);
  delay(10000);

  imu.dmpUpdateFifo();
  imu.updateAccel();
  optX_last = clamp(imu.calcAccel(imu.ax), -1.0, 1.0);
  optY_last = clamp(imu.calcAccel(imu.ay), -1.0, 1.0);
  optZ_last = clamp(imu.calcAccel(imu.az), -1.0, 1.0);

  for (unsigned i = 0; i < 30; ++i) {
    imu.dmpUpdateFifo();
    updateData(1.0f);
  }

  last_time_dt = micros();
  last_time_interval = micros();

}

void kalman_filter() {
  imu.updateAccel();

  optX = K * clamp(imu.calcAccel(imu.ax), -1.0, 1.0) + (1 - K) * optX_last;
  optY = K * clamp(imu.calcAccel(imu.ay), -1.0, 1.0) + (1 - K) * optY_last;
  optZ = K * clamp(imu.calcAccel(imu.az), -1.0, 1.0) + (1 - K) * optZ_last;

  optX_last = optX;
  optY_last = optY;
  optZ_last = optZ;
}

String getData() {
  String data = "";

  data += String(quat[0], 6) + ' ';
  data += String(quat[0], 6) + ' ';
  data += String(quat[0], 6) + ' ';
  data += String(quat[0], 6) + ' ';


  data += String(optX, 6) + ' ';
  data += String(optY, 6) + ' ';
  data += String(optZ, 6);
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
  imu.dmpUpdateFifo();

  quat[0] = imu.calcQuat(imu.qw);
  quat[1] = imu.calcQuat(imu.qx);
  quat[2] = imu.calcQuat(imu.qy);
  quat[3] = imu.calcQuat(imu.qz);

  kalman_filter();

  // Quaternion:
  // float q[4] = { qw, qx, qy, qz };

  // Quaternion on vector:
  float qv[4] = { 0.0f, optX, optY, optZ };
  float qrv[4];
  quat_mult(quat, qv, qrv);

  // Last on conjugate:
  float qc[4];
  quat_conj(quat, qc);
  float qrc[4];
  quat_mult(qrv, qc, qrc);

  optX = qrc[1] * dt;
  optY = qrc[2] * dt;
  otpZ = (qrc[3] - 1.0f) * dt

  // float cl = 0.001f;
  // float va = 0.0f;
  // set_in_range(ar[0], va, cl, -cl);
  // set_in_range(ar[1], va, cl, -cl);
  // set_in_range(ar[2], va, cl, -cl);

  //Serial.println("AR: " + String(ar[0], 6) + ' ' + String(ar[1], 6) + ' ' + String(ar[2], 6));
}

void connect(const char *SSID, const char *PASSWORD) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }

  Serial.println();
  Serial.println("Connected to the WiFi");
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  while (!client.connect(SERVER_IP, SERVER_PORT)) {
    Serial.println("Connection failed");
    delay(2000);
  }

  Serial.println();
  Serial.println("Connected to the server");
  delay(3000);
}
