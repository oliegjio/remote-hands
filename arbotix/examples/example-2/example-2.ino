#include "ax12.h"
#include "BioloidController.h"

void setup () {
  Serial.begin(9600);
}

void loop () {
  Serial.println("hello"); 
  ax12SetRegister2(254, 32, 150); //broadcast move speed
  ax12SetRegister2(254, 30, 50);  //broadcast move
  delay(1000);
}
