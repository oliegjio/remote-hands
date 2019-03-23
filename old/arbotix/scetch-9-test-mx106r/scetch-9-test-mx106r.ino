#include "MX106R.h"

#define count_servo 4

MX106 mx106;

int input_data[count_servo];

void zero_position() {
  for (unsigned char ID = 0; ID < count_servo; ++ID)
    mx106.turnAngleSpeed(ID, 0, 100);
}

void convertData(String data) {
  String tmp = "";
  for (int iter = 0, j = 0; iter < data.length(); ++iter) {
      if (data[iter] == '.') {
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
  int zeros[count_servo] = { 2525, 2080, 2048, 2015 };
  mx106.begin(baud, d_pin, &Serial1, count_servo, zeros);
  zero_position();
}

void loop(){
  // readFromSerial();
  // for (unsigned char ID = 0; ID < count_servo; ++ID)
  //   mx106.turnAngleSpeed(ID, input_data[ID], 100);
}
