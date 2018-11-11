#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "quaternionFilters.h"
#include "MPU9250.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>


#define port 1337
#define RED_LED_PIN D3
#define GREEN_LED_PIN D4
#define BLUE_LED_PIN D5
#define CALIBRATION_BUTTON D6
#define CONNECT_BUTTON D7

WiFiClient client;

const char *ssid  = "K-Lab";
const char *password = "allhailklab";
const String id = "1";



// I2C at 0x68

MPU9250 myIMU;

// Forward functions
String getValues();
void calibration();
void connect(const char *ssid, const char *password);
void blink(int pin);


void setup() {

    Wire.begin();
    Serial.begin(38400);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(CALIBRATION_BUTTON, INPUT);
    pinMode(CONNECT_BUTTON, INPUT);

    for(int i = 0; i < 10; i++){
        blink(GREEN_LED_PIN);
    }

    calibration();

    // setup Wifi
    connect(ssid, password);
    client.println('>' + id);

}

void loop() {
    if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
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

    client.print(getValues());
  }


    if(digitalRead(CALIBRATION_BUTTON)){
        calibration();
    }
    if(digitalRead(CONNECT_BUTTON)){
        connect(ssid, password);
    }

    if(WiFi.status() != WL_CONNECTED){

    }

    delay(100);
}

String getValues()
{
    String data = id + '|';
    data += String(myIMU.ax) + '|';
    data += String(myIMU.ay) + '|';
    data += String(myIMU.az) + '|';

    data += String(myIMU.gx) + '|';
    data += String(myIMU.gy) + '|';
    data += String(myIMU.gz);


    return data;
}

void calibration()
{
    analogWrite(BLUE_LED_PIN, 255);
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
    blink(GREEN_LED_PIN);
}else{
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);
    analogWrite(RED_LED_PIN, 255);
    while(1) ; // Loop forever if communication doesn't happen
}
}

void blink(int pin)
{
    analogWrite(GREEN_LED_PIN, 0);
    analogWrite(RED_LED_PIN, 0);
    analogWrite(BLUE_LED_PIN, 0);

    analogWrite(pin, 255);
    delay(500);
    analogWrite(pin, 0);
    delay(500);
}

void connect(const char *ssid, const char *password)
{
    WiFi.mode(WIFI_OFF);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        blink(BLUE_LED_PIN);
    }
    Serial.println();
    Serial.println("Connected to the WiFi");
    Serial.println(ssid);
    Serial.println(WiFi.localIP());
    
        client.connect(IPAddress(192,168,1,154), port);
        Serial.print(".");
        delay(500);
        blink(RED_LED_PIN);



    Serial.println();
    Serial.println("Connected to the Server");

}
