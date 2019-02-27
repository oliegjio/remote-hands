#include "MX106R.h"       // Library needed to control Dynamixal servo

#define SERVO_ID 0               // ID of Dynamixel (0x01 is default)
#define SERVO_ControlPin 2       // Control pin of buffer chip ( 74LS241N )u
#define SERVO_SET_Baudrate 1000000    // Baud rate speed which the Dynamixel will be set too (57600 is default)

MX106 mx106;

void setup(){
  Serial.begin(9600);
  Serial.println("Start");
  delay(1000);                                               // Give time for Dynamixel to start on power-up
  unsigned char p = 0x02;
  long baud = 1000000;
  mx106.begin(baud, p, &Serial1);                       // Set up Arduino to communicate to Dynamixel
}

int i = 0;

//255 255 1 2 1 251

//255 255 2 2 1 250
 
void loop(){ 
//  int ping = ax12a.ping(2); 
//  if (ping == 0){  // Ping SUCCESSFUL, Status packet has been recived with no error - the value returned from ping is the servo ID
//    Serial.println("Found: ");
//      }else{                                  // Ping NOT successful
//        Serial.println(ping);
//      }
//  ++i;
//  int res = ax12a.move(0, 1024);
//  Serial.print(i);
//  Serial.print(" : ");
//  Serial.println(res);
//  delay(1500);  // Delay 0.5 Sec
//  res = ax12a.move(0, 4000);
//  Serial.println(res);
//  delay(1500);
  int a = 4000;
  mx106.move(0, a);
  delay(1000);
  mx106.move(1, a);
  delay(1000);
  mx106.move(2, a);
  delay(1000);
  mx106.move(3, a);
  delay(1000);
  mx106.move(4, a);
  delay(1000);
  mx106.move(5, a);
  delay(1000);
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void setup() {
//  pinMode(2, OUTPUT);
//  pinMode(10, OUTPUT);
//  Serial.begin(9800);
//  Serial1.begin(9800);
//  Serial2.begin(9800);
//}
//
//void loop() {
//  Serial.println("From Serial1 to Serial2");
//  digitalWrite(10, LOW);
//  Serial1.write(0x0A);
//  digitalWrite(10, HIGH);
//  delay(100);
//  delay(100);
//  digitalWrite(2, HIGH);
//  if (Serial2.available() > 0) {  //если есть доступные данные
//        // считываем байт
//        int incomingByte = Serial2.read();
// 
//        // отсылаем то, что получили
//        Serial.print("I received: ");
//        Serial.println(incomingByte);
//    }
//   digitalWrite(2, LOW);
//   Serial1.flush();
//}
