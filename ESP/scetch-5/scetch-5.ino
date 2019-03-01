#include <SPI.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include "SparkFunMPU9250-DMP.h"

extern HardwareSerial Serial;

WiFiClient client;
MPU9250_DMP imu;

const char *SSID = "MS-7971";
const char *PASSWORD = "UfnbalFT";

#define SERVER_PORT 7247
#define SERVER_IP IPAddress(10, 42, 0, 1)

float currx  = 0;
float lastx1 = 0;
float lastx2 = 0;
float curry  = 0;
float lasty1 = 0;
float lasty2 = 0;
float currz  = 0;
float lastz1 = 0;
float lastz2 = 0;

float clamp(float value, float min_value, float max_value);
String getData();
void connect(const char *SSID, const char *PASSWORD);

float dt = 0.0f;
float last_time_dt;
unsigned long last_time_interval;
unsigned long interval = 100000.0f;

float x = 0.0f;
float y = 0.0f;
float z = 0.0f;

void setup() {
  Wire.begin();
  Serial.begin(115200);

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
  delay(8000);
  
  imu.updateAccel();
  lastx2 = clamp(imu.calcAccel(imu.ax), -1.0, 1.0);
  lasty2 = clamp(imu.calcAccel(imu.ay), -1.0, 1.0);
  lastz2 = clamp(imu.calcAccel(imu.az), -1.0, 1.0);

  imu.updateAccel();
  lastx1 = clamp(imu.calcAccel(imu.ax), -1.0, 1.0);
  lasty1 = clamp(imu.calcAccel(imu.ay), -1.0, 1.0);
  lastz1 = clamp(imu.calcAccel(imu.az), -1.0, 1.0);
  
  for (unsigned i = 0; i < 30; ++i) {
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

  if (micros() - last_time_interval > interval) {
    last_time_interval = micros();

    String data = getData();
    Serial.println(data);
//    client.println(data);

    clearData();
  }

  updateData(dt);
}

float clamp(float value, float min_value, float max_value) {
  value = ( value > max_value ) ? max_value : value;
  value = ( value < min_value ) ? min_value : value;
  return value;
}

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

float quat_mag(const float *q) {
  return q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
}

void quat_conj(const float *q, float *result) {
  result[0] = q[0];
  result[1] = -q[1];
  result[2] = -q[2];
  result[3] = -q[3];
}

bool quat_inv(const float *q, float *result) {
  float mag = quat_mag(q);
  if (mag == 0) return false;
  
  float conj[4];
  quat_conj(q, conj);
  
  result[0] = conj[0] / mag;
  result[1] = conj[1] / mag;
  result[2] = conj[2] / mag;
  result[3] = conj[3] / mag;
  
  return true;
}

void updateData(float dt) {
  imu.dmpUpdateFifo();

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

  imu.updateAccel();
  currx = clamp(imu.calcAccel(imu.ax), -1.0, 1.0);
  curry = clamp(imu.calcAccel(imu.ay), -1.0, 1.0);
  currz = clamp(imu.calcAccel(imu.az), -1.0, 1.0);

  float ax = (currx + lastx1 + lastx2) / 3;
  float ay = (curry + lasty1 + lasty2) / 3;
  float az = (currz + lastz1 + lastz2) / 3;

  // Inverse quaternion:
  float q[4] = { qw, qx, qy, qz };
  float qi[4];
  if (!quat_inv(q, qi)) {
    return;
  }

//  float qi[4] = { qw, qx, qy, qz };

  // Quaternion on vector:
  float qv[4] = { 0.0f, ax, ay, az };
  float qrv[4];
  quat_mult(qi, qv, qrv);

  // Last on conjugate:
  float qc[4];
  quat_conj(qi, qc);
  float qrc[4];
  quat_mult(qrv, qc, qrc);

  float ar[3] = { qrc[1] * dt, qrc[2] * dt, (qrc[3]) * dt };

  float cl = 0.00001;
  ar[0] = ar[0] > cl || ar[0] < -cl ? ar[0] : 0.0f;
  ar[1] = ar[1] > cl || ar[1] < -cl ? ar[1] : 0.0f;
  ar[2] = ar[2] > cl || ar[2] < -cl ? ar[2] : 0.0f;

  Serial.println(String(ar[0], 6) + ' ' + String(ar[1], 6) + ' ' + String(ar[2], 6));

  x += ar[0];
  y += ar[1];
  z += ar[2];
}

void clearData() {
  x = 0;
  y = 0;
  z = 0;
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
