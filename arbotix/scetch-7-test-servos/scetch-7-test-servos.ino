
// Arduino IDE v 1.8.7

#include "AX12A.h"

#define DirectionPin  (10u)
#define BaudRate      (1000000ul)
#define Broadcast     (254u)
#define MAX_ANGLE     (110)

#define ID1 4
#define ID2 2
#define ID3 1
#define ID4 3

AX12A ax12a2;

int Cur_angle[4] = {0, 0, 0, 0};
long input_data[3] = {0, 0, 0};

void correct_position(int & pos) {
    pos = (pos < -MAX_ANGLE) ? -MAX_ANGLE : pos;
    pos = (pos > MAX_ANGLE) ? MAX_ANGLE : pos;
}

void setup() {
    Serial.begin(9600);
    Serial3.begin(9600);
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

void start_position(int start_angle = 0) {
    for (int i = 1; i <= 5; ++i)
      turnAngle(i, start_angle, 100); 
}

// 40.6 mm 
// 71.2 mm
// 94.7 mm

void loop() {
    start_position();
    for (unsigned i = 1; i <= 5; ++i) {
        Serial.println(i);
        turnAngle(i, 90, 100);
        delay(1500);
        turnAngle(i, 0, 100);
        delay(1500);
    }
//    for (unsigned i = 0; i < 10; ++i) {
//        Serial.println(2);
//        turnAngle(ID2, 90, 100);
//        delay(1500);
//        turnAngle(ID2, 0, 100);
//        delay(1500);
//        turnAngle(ID2, -90, 100);
//        delay(1500);
//    }
//    for (unsigned i = 1; i <= 10; ++i) {
//        Serial.println(3);
//        turnAngle(ID3, -90, 100);
//        delay(1500);
//        turnAngle(ID3, 0, 100);
//        delay(1500);
//    }
//    for (unsigned i = 1; i <= 10; ++i) {
//        Serial.println(i);
//        turnAngle(ID4, -90, 100);
//        delay(1500);
//        turnAngle(ID4, 0, 100);
//        delay(1500);
//    }
}
