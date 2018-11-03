//Arduino v1.0.6

#include "ax12.h"
//#include "RemoteHand.h"

#define Broadcast 254

int movePosition(unsigned char id, int Position) {
  long long int Position_H, Position_L;
  Position_H = Position >> 8;
  Position_L = Position;
  
  ax12SetRegister2(id, 30, Position_L);
  ax12SetRegister2(id, 31, Position_H);
  return 1;
}

void setup () {
  Serial.begin(9600);

        
}

void loop () {
  ax12SetRegister(1, 47, 1);
  ax12SetRegister(2, 47, 1);
  ax12SetRegister(3, 47, 1);
  ax12SetRegister(4, 47, 1);
  Serial.println("hello");
  PlayTone(1, 1000);
  while (Serial.read() < 0);
  for (int j = 1; j <= 4; ++j) {
         for (int i = 300; i < 700; i += 100) {
           movePosition(j, i);
           delay(2000);
        } 
        movePosition(j, 300);
        delay(2000);
        movePosition(j, 512);
        delay(2000);
  }
  delay(1000);
  Serial.println("Good work");

}
