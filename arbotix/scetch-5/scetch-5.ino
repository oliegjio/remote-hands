//Arduino v1.0.6

#include "ax12.h"
#include "RemoteHand.h"

#define Broadcast 254

RemoteHand d1;

void setup () {
  Serial.begin(9600);       
}

void loop () {
  Serial.println("hello");
  d1.startPosition(1);
  delay(1500);
  d1.startPosition(2);
  delay(1500);
  d1.startPosition(3);
  delay(1500);
  d1.startPosition(4);
  delay(1500);
  while (Serial.read() < 0);
  Serial.println("Good work");

}
