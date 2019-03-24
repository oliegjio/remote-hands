#include "MX106R.h"

#define count_servo 4
#define min_angle   -45
#define max_angle   45

MX106 mx106;

int input_data[count_servo];

void goToZeroPosition() {
  for (unsigned char ID = 0; ID <= count_servo; ++ID)
    mx106.turnAngleSpeed(ID, 0, 100);
}

void convertData(String data) {
  String tmp = "";
  for (int iter = 0, j = 0; iter < data.length() && j < count_servo; ++iter) {
      if (data[iter] == '.' || data[iter] == ' ') {
          while (data[iter] != ' ') iter++;
          input_data[j] = tmp.toInt();
          tmp = ""; ++j;
      } else {
          tmp += data[iter];
      }
  }
}

void readFromSerial() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    convertData(data);
  }
}

void setup(){
  Serial.begin(9600);           // input Serial
  Serial2.begin(9600);          // output Serial
  Serial2.println("Start");
  delay(1000);
  unsigned char d_pin = 0x02;
  long baud = 1000000;
  int zeros[count_servo] = { 1560, 2080, 2048, 2002 };
  mx106.begin(baud, d_pin, &Serial1, count_servo, zeros, min_angle, max_angle);
  goToZeroPosition();
  delay(1000);
}

void loop(){
  // mx106.turnAngleSpeed(0, 10, 100);
  // mx106.turnAngleSpeed(1, -60, 100);
  // mx106.turnAngleSpeed(2, -60, 100);
  // mx106.turnAngleSpeed(3, -60, 100);

  // String s = "-1.0 -1 0 50 \n";
  // convertData(s);
  // for (unsigned iter = 0; iter < count_servo; ++iter) {
  //   Serial.print(input_data[iter]);
  //   Serial.print(' ');
  // }
  // Serial.println();

  readFromSerial();
  for (unsigned char ID = 0; ID < count_servo; ++ID) {
    int k = -1;
    if (ID % 2) k = 1;
    int val = map(k * input_data[ID], -90, 90, -45, 45);
    Serial2.print(val);
    Serial2.print(' ');
    mx106.turnAngleSpeed(ID, val, 100);
  }
  Serial2.println();
  delay(10);
}
