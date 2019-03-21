#include "MX106R.h"

#define count_data 3

MX106 mx106;

int input_data[count_data];

void setup(){
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.println("Start");
  delay(1000);
  unsigned char d_pin = 0x02;
  long baud = 1000000;
  mx106.begin(baud, d_pin, &Serial1);
}

void convertData(String data) {
  String tmp = "";
  for (int iter = 0, j = 0; iter < data.length(); ++iter) {
      if (data[iter] == '.') {
          while (data[iter] != ' ') iter++;
          input_data[j] = tmp.toInt();
          tmp = ""; ++j;
      } else {
          tmp += data[iter];
      }
  }
}

void readFromSerial() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    convertData(data);
  }
}

void loop(){
  Serial.println("Loop");
  mx106.turnAngle(2, -90);
  delay(1000);
  for (int i = -90; i < 90; i += 5) {
    mx106.turnAngle(2, i);
    delay(100);
  }
}
