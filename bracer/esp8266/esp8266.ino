#include <WiFiClient.h>
#include <ESP8266WiFi.h>

#define SERVER_PORT 7247
#define SERVER_IP   IPAddress(10, 42, 0, 1)

WiFiClient client;

const char *WIFI_SSID = "MS-7971";
const char *WIFI_PASSWORD = "UfnbalFT";

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  while (!client.connect(SERVER_IP, SERVER_PORT)) {
    delay(1500);
  }
}

void loop() {
  while (Serial.available() > 0) {
    client.print((char) Serial.read());
  }
}
