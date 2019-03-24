#include "MX106R.h"

#define count_servo 4
#define min_angle   -90
#define max_angle   90

MX106 mx106;

int input_data[count_servo];

void goToZeroPosition() {
  for (unsigned char ID = 0; ID < count_servo; ++ID)
    if (ID == 0)
      mx106.turnAngleSpeed(ID, 180, 100);
    else if (ID == 1)
      mx106.turnAngleSpeed(ID, 90, 100);
    else 
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
    Serial2.println(data);
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
  int zeros[count_servo] = { 2048, 1574, 2435, 1395 };
  mx106.begin(baud, d_pin, &Serial1, count_servo, zeros, min_angle, max_angle);
  goToZeroPosition();
  delay(3000);
  while (!Serial.available());
}

void loop(){
  readFromSerial();
  for (unsigned char ID = 0; ID < count_servo; ++ID) {
    int k;
    if (ID == 0) k = 1;
    if (ID == 1) k = 1;
    if (ID == 2) k = -1;
    if (ID == 3) k = 1;
    Serial2.print(input_data[ID]);
    Serial2.print(' ');
    mx106.turnAngleSpeed(ID, k * input_data[ID], 100);
  }
  Serial2.println();
  delay(10);
}
