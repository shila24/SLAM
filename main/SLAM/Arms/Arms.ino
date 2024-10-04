#include <Servo.h>
const int servoPin0 = 6; 
const int servoPin1 = 7; 


Servo mortor0;
Servo mortor1;
int pos0 = 0; 
int pos1 = 0;



void setup() {
  // put your setup code here, to run once:
  mortor0.attach(servoPin0);
  mortor1.attach(servoPin1);


}

void loop() 
{
 for (pos0 = 0; pos0 < 90; pos0 += 1) {
 mortor0.write(pos0);
 mortor1.write(180-pos0);
 delay(10);
 }
 
 for (pos0 = 90; pos0 >= 1; pos0 -= 1) {
 mortor0.write(pos0);
 mortor1.write(180-pos0);
 delay(10);
 }
 }
 

 
