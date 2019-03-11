#include "MX106R.h"

MX106 mx106;

void setup(){
  Serial.begin(9600);
  Serial.println("Start");
  delay(1000);
  unsigned char d_pin = 0x02;
  long baud = 1000000;
  mx106.begin(baud, d_pin, &Serial1);
}

void loop(){
  Serial.println("Loop");
  mx106.turnAngle(0, -90);
  delay(1000);
  for (int i = -90; i < 90; i += 5) {
    mx106.turnAngle(0, i);
    delay(100);
  }
}
