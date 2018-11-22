
// Arduino IDE v 1.8.7

#include "AX12A.h"

#define DirectionPin  (10u)
#define BaudRate      (1000000ul)
#define Broadcast     (254u)
#define MAX_ANGLE     (110)

int Cur_angle[4] = {0, 0, 0, 0};
long input_data[3] = {0, 0, 0};

void correct_position(int & pos) {
    pos = (pos < -MAX_ANGLE) ? -MAX_ANGLE : pos;
    pos = (pos > MAX_ANGLE) ? MAX_ANGLE : pos;
}

void setup() {
    Serial.begin(9600);
    Serial3.begin(9600);
    ax12a.begin(BaudRate, DirectionPin, &Serial1);
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
    for (int i = 1; i <= 4; ++i)
      turnAngle(i, start_angle, 100); 
}


void convert_data(String data) {
    String tmp = "";
    for (int iter = 0, j = 0; iter < data.length(); ++iter) {
        if (data[iter] == ' ') {
            input_data[j] = tmp.toInt();
            tmp = ""; ++j;
        } else {
            tmp += data[iter];
        }
    }
}

// 40.6 mm 
// 71.2 mm
// 94.7 mm

void loop() {
    //Serial.println("Calibrate");
    //start_position();
    while (1) {
        if (Serial.available() > 0) {
            String data = Serial.readStringUntil('\n');
            //Serial3.println(data);
            convert_data(data);
            for (int i = 0; i < 3; ++i) {
                Serial3.print(input_data[i]); Serial3.print(' ');
            }
            Serial3.println();
            for (unsigned char iter = 1; iter <= 3; ++iter)
                turnAngle(iter, input_data[iter - 1], 200);
        }
    }
}
