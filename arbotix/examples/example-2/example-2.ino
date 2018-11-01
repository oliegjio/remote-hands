#include <AX12A.h>

#define DirectionPin  (10u)
#define BaudRate    (1000000ul)
#define ID    (4u)
#define ID2   (1u)

void setup()
{
  Serial.begin(9600);
 ax12a.begin(BaudRate, DirectionPin, &Serial2);
 //ax12a1.begin(BaudRate, DirectionPin, &Serial2);
 ax12a.setEndless(ID, ON);
}

void loop()
{
  Serial.println("Hello");
  while(Serial.read() < 0);
  ax12a.moveSpeed(254, 512, 100);
}
