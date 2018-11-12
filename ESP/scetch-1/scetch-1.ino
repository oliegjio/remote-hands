#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "quaternionFilters.h"
#include "MPU9250.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

MPU9250 myIMU;

WiFiClient client;
const char *ssid = "vadya_forester";
const char *password = "vadya2001";

void connectToServer(const char *ssid, const char *password) {
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected complite, IP address: ");
  Serial.println(WiFi.localIP());
}

//9250

void calibration()
{
    byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
    Serial.print(" I should be "); Serial.println(0x71, HEX);

    if (c == 0x71) // WHO_AM_I should always be 0x68
    {
    Serial.println("MPU9250 is online...");

    // Start by performing self test and reporting values
    myIMU.MPU9250SelfTest(myIMU.SelfTest);
    Serial.print("x-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[0],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[1],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: acceleration trim within : ");
    Serial.print(myIMU.SelfTest[2],1); Serial.println("% of factory value");
    Serial.print("x-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[3],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[4],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: gyration trim within : ");
    Serial.print(myIMU.SelfTest[5],1); Serial.println("% of factory value");

    // Calibrate gyro and accelerometers, load biases in bias registers
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);

    myIMU.initMPU9250();
    Serial.println("MPU9250 initialized for active data mode....");

    byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX);
    Serial.print(" I should be "); Serial.println(0x48, HEX);

    myIMU.initAK8963(myIMU.magCalibration);
    // Initialize device for active mode read of magnetometer
    Serial.println("AK8963 initialized for active data mode....");


    Serial.print("X-Axis sensitivity adjustment value ");
    Serial.println(myIMU.magCalibration[0], 2);
    Serial.print("Y-Axis sensitivity adjustment value ");
    Serial.println(myIMU.magCalibration[1], 2);
    Serial.print("Z-Axis sensitivity adjustment value ");
    
    Serial.println(myIMU.magCalibration[2], 2);
    }else{
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);
    while(1) ; // Loop forever if communication doesn't happen
}
}
 
void setup() {
  Serial.begin(115200);
  Serial.println();
  connectToServer(ssid, password);
  myIMU.initMPU9250();
  myIMU.writeByte(0x68, WHO_AM_I_MPU9250, 0x71);
  Serial.println(myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250));
  delay(1000000);
  //calibration();
}

void loop() {
//  Vector rawGyro = mpu.readRawGyro();
//  Vector normGyro = mpu.readNormalizeGyro();
// 
//  Serial.print(" Xraw = ");
//  Serial.print(rawGyro.XAxis);
//  Serial.print(" Yraw = ");
//  Serial.print(rawGyro.YAxis);
//  Serial.print(" Zraw = ");
//  Serial.println(rawGyro.ZAxis);
// 
//  Serial.print(" Xnorm = ");
//  Serial.print(normGyro.XAxis);
//  Serial.print(" Ynorm = ");
//  Serial.print(normGyro.YAxis);
//  Serial.print(" Znorm = ");
//  Serial.println(normGyro.ZAxis);
  
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values
    myIMU.getAres();

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0]*myIMU.aRes; // - accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1]*myIMU.aRes; // - accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2]*myIMU.aRes; // - accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values
    myIMU.getGres();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes;

    Serial.print(myIMU.ax); Serial.print(" | ");
    Serial.print(myIMU.ay); Serial.print(" | ");
    Serial.print(myIMU.az); Serial.print(" | ");
    Serial.print(myIMU.gx); Serial.print(" | ");
    Serial.print(myIMU.gy); Serial.print(" | ");
    Serial.print(myIMU.gz); Serial.print(" | ");
    Serial.println();
    
    //client.print(getValues());
  }
  delay(100);
}
