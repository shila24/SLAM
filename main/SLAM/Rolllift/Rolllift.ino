#include <Servo.h>
const int servoPin0 = 5; 
const int servoPin1 = 6; 
const int liftpin = 7;
Servo mortor0;
Servo mortor1;
Servo lmortor;

int pos0 = 0; 
int pos1 = 0;



void setup() {
  // put your setup code here, to run once:
  mortor0.attach(servoPin0);
  mortor1.attach(servoPin1);
  lmortor.attach(liftpin);   //PWMにつなぐ。[~]マークの3,5,6,9,10,11。ここでは6。
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


void loop() {
roll.write(80);    //ここの数値が正転・反転、速度を司る。
delay(100);
roll.write(90); 
delay(100);
roll.write(100); 
}
