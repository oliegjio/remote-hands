#include "ax12.h"
#include "BioloidController.h"

void setup () {
  Serial.begin(9600);
}

void loop () {
  while (Serial.read() < 0);
  Serial.println("hello");
  ax12SetRegister2(2, 32, 500);
  ax12SetRegister2(2, 30, 50); 
  delay(1000);
  while (ax12GetRegister(2, 36, 2) != 1023) {
    Serial.println(ax12GetRegister(2, 36, 2));
  }
  ax12SetRegister2(2, 32, 0);
  Serial.println("Good work");
  delay(10000);
}
