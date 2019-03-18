#include <SPI.h>
#include <Wire.h>
#include "SparkFunMPU9250-DMP.h"

#define K 0.05

MPU9250_DMP imu;

double optX;
double optY;
double optZ;

double optX_last;
double optY_last;
double optZ_last;

float clamp(float value, float min_value, float max_value);
String getData();

void setup() {
  Wire.begin();
  Serial.begin(115200);

  if (imu.begin() != INV_SUCCESS) {
    while (1) {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);

  while ( !imu.fifoAvailable() && !imu.dmpUpdateFifo() == INV_SUCCESS );  
  delay(12000);

  imu.dmpUpdateFifo();
  imu.updateAccel();
  optX_last = clamp(imu.calcAccel(imu.ax), -1.0, 1.0);
  optY_last = clamp(imu.calcAccel(imu.ay), -1.0, 1.0);
  optZ_last = clamp(imu.calcAccel(imu.az), -1.0, 1.0);

  Serial.println("Start loop");
}

void loop() {
  imu.dmpUpdateFifo();
  imu.updateAccel();
  
  optX = K * clamp(imu.calcAccel(imu.ax), -1.0, 1.0) + (1 - K) * optX_last;
  optY = K * clamp(imu.calcAccel(imu.ay), -1.0, 1.0) + (1 - K) * optY_last;
  optZ = K * clamp(imu.calcAccel(imu.az), -1.0, 1.0) + (1 - K) * optZ_last;

  optX_last = optX;
  optY_last = optY;
  optZ_last = optZ;

  Serial.println(getData());
  Serial.flush();
}

float clamp(float value, float min_value, float max_value) {
  value = ( value > max_value ) ? max_value : value;
  value = ( value < min_value ) ? min_value : value;
  return value;
}

String getData() {
  String data = "";
  data += String(optX) + ' ';
  data += String(optY) + ' ';
  data += String(optZ);
  return data;
}
