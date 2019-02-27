#include <SPI.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include "SparkFunMPU9250-DMP.h"

WiFiClient client;
MPU9250_DMP imu;

const char *SSID = "MS-7971";
const char *PASSWORD = "UfnbalFT";

#define PORT   7247
#define IP     IPAddress(10, 42, 0, 1)

float currx  = 0;
float lastx1 = 0;
float lastx2 = 0;
float curry  = 0;
float lasty1 = 0;
float lasty2 = 0;
float currz  = 0;
float lastz1 = 0;
float lastz2 = 0;

extern HardwareSerial Serial;

float clamp(float value, float min_value, float max_value);
String getData();
void connect(const char *SSID, const char *PASSWORD);

float dt = 0.0f;
float last_time_dt;
float last_time_interval;
float interval = 10000.0f;

float qw = 0.0f;
float qx = 0.0f;
float qy = 0.0f;
float qz = 0.0f;
float x = 0.0f;
float y = 0.0f;
float z = 0.0f;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  connect(SSID, PASSWORD);

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
      getData();
  }

  last_time_dt = micros();
  last_time_interval = micros();

  Serial.println("Start loop");
}

void loop() {
  dt = (micros() - last_time_dt) / 1000000.0f;
  last_time_dt = micros();

  Serial.println(dt, 6);

  if (micros() - last_time_interval > interval) {
    last_time_interval = micros();

    String data = getData();
    Serial.println(data);
    client.println(data);

    clearData();
  }

  imu.dmpUpdateFifo();
  updateData(dt);
}

///////////////////////////// ///////////////////////////////////
/////             Desctiption functions                    //////
/////////////////////////////////////////////////////////////////

float clamp(float value, float min_value, float max_value) {
  value = ( value > max_value ) ? max_value : value;
  value = ( value < min_value ) ? min_value : value;
  return value;
}

String getData() {
  String data = "";

  data += String(qw) + ' ';
  data += String(qx) + ' ';
  data += String(qy) + ' ';
  data += String(qz) + ' ';
  data += String(x) + ' ';
  data += String(y) + ' ';
  data += String(z);

  return data;
}

void updateData(float dt) {
  // float qwn = imu.calcQuat(imu.qw);
  // float qxn = imu.calcQuat(imu.qx);
  // float qyn = imu.calcQuat(imu.qy);
  // float qzn = imu.calcQuat(imu.qz);

  // qw = -qx * qxn - qy * qyn - qz * qzn + qw * qwn;
  // qx = qx * qwn + qy * qzn - qz * qyn + qw * qxn;
  // qy = -qx * qzn + qy * qwn + qz * qxn + qw * qyn;
  // qz =  qx * qyn - qy * qxn + qz * qwn + qw * qzn;

  qw = imu.calcQuat(imu.qw);
  qx = imu.calcQuat(imu.qx);
  qy = imu.calcQuat(imu.qy);
  qz = imu.calcQuat(imu.qz);

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
  
  x += ax * dt;
  y += ay * dt;
  z += az * dt;
}

void clearData() {
  qw = 0;
  qx = 0;
  qy = 0;
  qz = 0;
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
  Serial.println(SSID);
  Serial.println(WiFi.localIP());

  while (!client.connect(IP, PORT)) {
    Serial.println("Connection failed");
    delay(2000);
  }

  Serial.println();
  Serial.println("Connected to the server");
  delay(3000);
}
