#include <ESP8266WiFi.h>
#include <MPU6050.h>

MPU6050 mpu;

const char* ssid = "ssid name";
const char* password = "ssid password";
 
int ledPin = D5;
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
}

void loop() {
  Serial.println("fuck");
}
