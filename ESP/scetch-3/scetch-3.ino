#include <SPI.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include "SparkFunMPU9250-DMP.h"


WiFiClient client;
MPU9250_DMP imu;

const char *ssid  = "mint-pc";
const char *password = "fFK5ieoY";

#define port 7247
#define ip   IPAddress(10,42,0,1)

//Forward functions
float clamp(float value, float min_value, float max_value);
String getData();
void connect(const char *ssid, const char *password);


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
}

void loop() {
    if ( imu.fifoAvailable() ) {
        if ( imu.dmpUpdateFifo() == INV_SUCCESS) {
              imu.updateAccel();
              imu.computeEulerAngles();
              Serial.println(getData());
              //client.println(getData());
         }
    }
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
    float q0 = imu.calcQuat(imu.qw);
    float q1 = imu.calcQuat(imu.qx);
    float q2 = imu.calcQuat(imu.qy);
    float q3 = imu.calcQuat(imu.qz);
    
    String data = String(q0) + ' ';
    data += String(q1) + ' ';
    data += String(q2) + ' ';
    data += String(q3) + ' ';

    float ax = clamp(imu.calcAccel(imu.ax), -1.0, 1.0);
    float ay = clamp(imu.calcAccel(imu.ay), -1.0, 1.0);
    float az = clamp(imu.calcAccel(imu.az), -1.0, 1.0);

    data += String(ax) + ' ';
    data += String(ay) + ' ';
    data += String(az) + ' ';

    return data;
}

void connect(const char *ssid, const char *password) {
    WiFi.mode(WIFI_OFF);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED){
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
