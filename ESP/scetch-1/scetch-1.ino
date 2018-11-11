#include <ESP8266WiFi.h>
#include <MPU6050.h>

MPU6050 mpu;

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
 
void setup() {
  Serial.begin(115200);
  Serial.println();
  connectToServer(ssid, password);

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Nie mozna znalezc MPU6050 - sprawdz polaczenie!");
    delay(500);
  }
  
  mpu.calibrateGyro();
 
  mpu.setThreshold(3);
}

void loop() {
  Vector rawGyro = mpu.readRawGyro();
  Vector normGyro = mpu.readNormalizeGyro();
 
  Serial.print(" Xraw = ");
  Serial.print(rawGyro.XAxis);
  Serial.print(" Yraw = ");
  Serial.print(rawGyro.YAxis);
  Serial.print(" Zraw = ");
  Serial.println(rawGyro.ZAxis);
 
  Serial.print(" Xnorm = ");
  Serial.print(normGyro.XAxis);
  Serial.print(" Ynorm = ");
  Serial.print(normGyro.YAxis);
  Serial.print(" Znorm = ");
  Serial.println(normGyro.ZAxis);
}
