#include <SPI.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include "SparkFunMPU9250-DMP.h"


WiFiClient client;
MPU9250_DMP imu;

const char *ssid  = "vadim-N56JRH";
const char *password = "sIRwHeaJ";

#define port   7247
#define ip     IPAddress(10,42,0,1)
#define BUTTON 16

float currx  = 0;
float lastx1 = 0;
float lastx2 = 0;
float curry  = 0;
float lasty1 = 0;
float lasty2 = 0;
float currz  = 0;
float lastz1 = 0;
float lastz2 = 0;

//Forward functions
float clamp(float value, float min_value, float max_value);
String getData();
void connect(const char *ssid, const char *password);
bool checkButton();

void setup() {
  Wire.begin();
  Serial.begin(115200);

  connect(ssid, password);

  if (imu.begin() != INV_SUCCESS) {
    while (1) {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);

  pinMode(BUTTON, INPUT);

  while ( !imu.fifoAvailable() && !imu.dmpUpdateFifo() == INV_SUCCESS );  
  delay(12000);
  
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
  Serial.println("Start loop");
}

void loop() {
  imu.dmpUpdateFifo();
  Serial.println(getData());
  client.println(getData());   

//  if (checkButton() == HIGH)
//    Serial.println("Hello");
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
  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

  data += String(q0) + ' ';
  data += String(q1) + ' ';
  data += String(q2) + ' ';
  data += String(q3) + ' ';

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
  
  data += String(ax) + ' ';
  data += String(ay) + ' ';
  data += String(az);

  /*String data = String(imu.pitch) + ' ';
    data += String(imu.roll) + ' ';
    data += String(imu.yaw);*/

  return data;
}

void connect(const char *ssid, const char *password) {
  WiFi.mode(WIFI_OFF);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }
  Serial.println();
  Serial.println("Connected to the WiFi");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());

  while (!client.connect(ip, port)) {
    Serial.println("connection failed");
    delay(2000);
  }

  Serial.println();
  Serial.println("Connected to the Server");
  delay(3000);
}

bool checkButton() {
  return digitalRead(BUTTON);
}
