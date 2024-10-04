#include <Servo.h>
const int servoPin0 = 7; //腕部分右
const int servoPin1 = 8; //腕部分左
const int servoPinL = 6; //釣り上げ
const int TRIG = 11; //音波センサトリガー
const int ECHO = 10; //音波センサエコー
Servo mortor0;
Servo mortor1;
Servo mortorL;
double duration = 100;
double distance = 100;
double speed_of_sound = 331.5 + 0.6 * 22;
int pos0 = 0; 
int pos1 = 0;

void setup()
{
  Serial.begin( 9600 );
  // put your setup code here, to run once:
  mortor0.attach(servoPin0);
  mortor1.attach(servoPin1);
  mortorL.attach(servoPinL);   //PWMにつなぐ。[~]マークの3,5,6,9,10,11。ここでは6。
  pinMode(ECHO, INPUT );
  pinMode(TRIG, OUTPUT );
  wave_lift(TRIG, ECHO);
}

//360°サーボモーターについて
//writeの数値が正転・反転、速度を司る。0-90正転、90-180反転数字が90から離れるほど早さ向上

void loop()
{
  
  
}

void liftup(int t1,int t2)
{//巻き上げの秒数、保持の秒数
  mortorL.write(135);
  delay(t1);
  mortorL.write(90);
  delay(100);
  for (pos0 = 90; pos0 >= 1; pos0 -= 1) {
    mortor0.write(180-pos0);
    mortor1.write(pos0);
    delay(5);
    }
  delay(50);
  mortorL.write(45); 
  delay(t1);
  mortorL.write(90);
  delay(t2);
  mortorL.write(135);
  delay(t1);
  mortorL.write(90);
  delay(50);
  for (pos0 = 0; pos0 < 90; pos0 += 1) {
    mortor0.write(180-pos0);
    mortor1.write(pos0);
    delay(5);
  }
  delay(100);
  mortorL.write(45); 
  delay(t1);
  mortorL.write(90);
  delay(1000);
  
  
}
void wave_lift(int TRIG, int ECHO){
  
  if (duration >= 0) {
    while (distance > 3.5){
      Serial.print("Distance:");
      Serial.print(distance);
      Serial.println(" cm");
      digitalWrite(TRIG, LOW); 
      delayMicroseconds(2);
      digitalWrite( TRIG, HIGH );
      delayMicroseconds( 10 );
      digitalWrite( TRIG, LOW );
      duration = pulseIn( ECHO, HIGH );
      duration = duration / 2; // 往路にかかった時間
      distance = duration * speed_of_sound * 100 / 1000000;}
    delay(1000);
    liftup(2500,3200);
    }
    }

    


