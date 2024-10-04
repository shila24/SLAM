#include <Servo.h>
const int servoPin0 = 7; //腕部分右
const int servoPin1 = 8; //腕部分左
const int servoPinL = 6; //釣り上げ
Servo mortor0;
Servo mortor1;
Servo mortorL;

int pos0 = 0; 
int pos1 = 0;

void setup()
{
  // put your setup code here, to run once:
  mortor0.attach(servoPin0);
  mortor1.attach(servoPin1);
  mortorL.attach(servoPinL);   //PWMにつなぐ。[~]マークの3,5,6,9,10,11。ここでは6。
}

//360°サーボモーターについて
//writeの数値が正転・反転、速度を司る。0-90正転、90-180反転数字が90から離れるほど早さ向上

void loop()
{
  liftup(100,200);
}

void liftup(int t1,int t2)
{//巻き上げの秒数、保持の秒数
  mortorL.write(70);
  delay(t1);
  mortorL.write(90);
  for (pos0 = 0; pos0 < 90; pos0 += 1) {
    mortor0.write(pos0);
    mortor1.write(180-pos0);
    delay(5);
    }
  mortorL.write(110); 
  delay(t1);
  mortorL.write(90);
  delay(t2);
  mortorL.write(70);
  delay(t1);
  mortorL.write(90);
  for (pos0 = 90; pos0 >= 1; pos0 -= 1) {
    mortor0.write(pos0);
    mortor1.write(180-pos0);
    delay(5);
  }
  mortorL.write(110); 
  delay(1000);
}




