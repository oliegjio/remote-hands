//Arduino v1.0.6

//Arbotix RIP T_T 03.11.2018

#include "ax12.h"
#include "RemoteHand.h"

#define Broadcast 254

RemoteHand d;

void start_program() {
  for (int i = 1; i <= 4; ++i) {
    d.startPosition(i); 
    delay(100); 
  }
}

void setup () {
  Serial.begin(9600);
  pinMode(15, OUTPUT);  
}

void loop () {
  digitalWrite(15, HIGH);
  start_program();
  Serial.println("hello");
  Serial.println("Good work");
}
