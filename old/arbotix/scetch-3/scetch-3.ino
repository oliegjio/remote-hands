#include <AX12A.h>

#define DirectionPin  (10u)
#define BaudRate      (1000000ul)
#define Broadcast     (254u)
#define MAX_ANGLE     (110)

int Cur_angle[4] = {};

void correct_position(int & pos) {
  pos = (pos < -MAX_ANGLE) ? -MAX_ANGLE : pos;
  pos = (pos > MAX_ANGLE) ? MAX_ANGLE : pos;
}

void setup() {
  Serial.begin(9600);
  ax12a.begin(BaudRate, DirectionPin, &Serial2);
  ax12a.setEndless(1, OFF);
  ax12a.setEndless(2, OFF);
  ax12a.setEndless(3, OFF);
  ax12a.setEndless(4, OFF);
}

void turnAngle(unsigned char id, int angle, int _speed) {
  correct_position(angle);
  Cur_angle[id - 1] = angle;
  ax12a.moveSpeed(id, static_cast <int> (512 + angle * 3.45), _speed);
}

void loop() {
  Serial.println("Calibrate");
  //while (Serial.read() < 0);
  turnAngle(1, 20, 100);
  delay(750);
  turnAngle(1, 40, 100);
  delay(750);
  turnAngle(1, 0, 200);
  delay(1000);
  turnAngle(1, 20, 100);
  delay(750);
  turnAngle(1, 40, 200);
  delay(750);
}
