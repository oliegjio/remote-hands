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
  for (unsigned char i = 0; i < 6; ++i)
    mx106.move(i, 1000);
}