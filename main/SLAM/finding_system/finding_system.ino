#include <Servo.h>
#include <AFMotor.h>
#define echoPin 50 // Echo Pin
#define trigPin 52 // Trigger Pin
#define servoPin 46 //servo Pin
#define point 30 //距離の基準
 
double Duration = 0; //受信した間隔
double Distance[9]; //距離 50°から130までの距離を10°ずつに9個の値を格納

Servo myservo;

AF_DCMotor motorleft(1);
AF_DCMotor motorright(2);

int pos[9]={50,60,70,80,90,100,110,120,130};  //サーボの角度　50°から130°までの9個値を格納

int i=0;  //角度のカウント関数

int N=0;   //90°の時の値
int R=0;   //60°の時の値
int R1=0;  //80°の時の値
int L=0;   //120°の時の値
int L1=0;  //100°の時の値

int q=0;   //状態関数　q=0->静止　q=1->直進
#include <Servo.h>
const int servoPin0 = 7; //腕部分右
const int servoPin1 = 8; //腕部分左
const int servoPinL = 6; //釣り上げ
const int TRIG = 11; //音波センサトリガー
const int ECHO = 10; //音波センサエコー
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
  wave_lift(TRIG, ECHO);
  
}

//360°サーボモーターについて
//writeの数値が正転・反転、速度を司る。0-90正転、90-180反転数字が90から離れるほど早さ向上

void loop()
{

  
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
void wave_lift(int TRIG, int ECHO){
  double duration = 0;
  double distance = 0;
  double speed_of_sound = 331.5 + 0.6 * 22;
  if (duration >= 0) {
    while (distance > 3){
      digitalWrite(TRIG, LOW); 
      delayMicroseconds(2);
      digitalWrite( TRIG, HIGH );
      delayMicroseconds( 10 );
      digitalWrite( TRIG, LOW );
      duration = pulseIn( ECHO, HIGH );
      duration = duration / 2; // 往路にかかった時間
      distance = duration * speed_of_sound * 100 / 1000000;
      Serial.print("Distance:");
      Serial.print(distance);
      Serial.println(" cm");}
    //liftup(100,200);
    }}
void mooov()