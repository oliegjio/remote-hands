#include <ax12.h>
#include <BioloidController.h>

BioloidController bioloid = BioloidController(1000000);

int R2;
int R4;
int R6;
int R8;

void setup(){
// Initially moving all servos to default position.
  SetPosition(1, 512);
  SetPosition(2, 512);
  SetPosition(3, 512);
  SetPosition(4, 512);
  SetPosition(5, 512);
  SetPosition(6, 512);
  SetPosition(7, 512);
  SetPosition(8, 512);
// Turning torque off of the servos on one arm.
  Relax(2);    
  Relax(4);
  Relax(6);
  Relax(8);
  delay(1000);
}

void loop(){
// Reading "Present Position" register value, writing it to variable
  R2 = ax12GetRegister(2, 36, 2); 
  R4 = ax12GetRegister(4, 36, 2);
  R6 = ax12GetRegister(6, 36, 2);
  R8 = ax12GetRegister(8, 36, 2);
// Using the "Present Position" values of one arm, writing them to other arm
  SetPosition(1, R2);
  SetPosition(3, R4);
  SetPosition(5, R6);  
  SetPosition(7, R8);  
  delay(20);
}