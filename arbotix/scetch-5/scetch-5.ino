//Arduino v1.0.6

#include "ax12.h"
#include "RemoteHand.h"

#define Broadcast 254

RemoteHand d;

void start_program() {
  for (int i = 1; i <= 4; ++i) {
    d.startPosition(i);
    //d.lockMotor(i); 
    delay(100); 
  }
  delay(3000);
}

void setup () {
  Serial.begin(9600);       
}

void loop () {
  start_program();
  Serial.println("hello");
  while (Serial.read() < 0);
  d.moveSpeed(4, 100, -90);
  delay(3000);
  d.moveSpeed(4, 100, 90);
  Serial.println("Good work");
  delay(10000);
}
