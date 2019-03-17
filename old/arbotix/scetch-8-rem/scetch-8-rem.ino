
// Arduino IDE v 1.8.7

#include "AX12A.h"

#define DirectionPin  (10u)
#define BaudRate      (1000000ul)
#define Broadcast     (254u)
#define MAX_ANGLE     (110)

AX12A ax12a2;

void correct_position(int & pos) {
    pos = (pos < -MAX_ANGLE) ? -MAX_ANGLE : pos;
    pos = (pos > MAX_ANGLE) ? MAX_ANGLE : pos;
}

void setup() {
    Serial.begin(9600);
    ax12a.begin(BaudRate, DirectionPin, &Serial1);
    ax12a.setEndless(1, OFF);
    ax12a.setEndless(2, OFF);
    ax12a.setEndless(3, OFF);
    ax12a.setEndless(4, OFF);
}

void turnAngle(unsigned char id, int angle, int _speed) {
    correct_position(angle);
    ax12a.moveSpeed(id, static_cast <int> (512 + angle * 3.45), _speed);
}

void start_position(int start_angle = 0) {
    for (int i = 1; i <= 4; ++i)
      turnAngle(i, start_angle, 100); 
}

void loop() {
    Serial.println("Calibrate");
    start_position();
    getch();
}
