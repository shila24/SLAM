#include "Servo.h"
const int Wall_Echo = 12; // Echo Pin
const int Wall_Trig = 13; // Trigger Pin
const int Shake_servoP = 9; //首振りservo Pin
const int LeftP1   = 4;//RIN
const int RightP1  = 5;//FIN
const int LeftP2   = 2;//RIN
const int RightP2  = 3;//FIN
const int point = 40; //距離の基準 
double speed_of_sound = 331.5 + 0.6 * 25;
Servo myservo;
double Duration = 0; //受信した間隔
double Distance[9]; //距離 50°から130までの距離を10°ずつに9個の値を格納
unsigned long now;
unsigned long starttime1 = 0;
unsigned long starttime2 = 0;

int pos[11]={45,54,63,72,81,90,99,108,117,126,135);  //サーボの角度　45°から135°までの9個値を格納

int i=0;  //角度のカウント関数

int N=0;   //90°の時の値
int R=0;   //60°の時の値
int R1=0;  //80°の時の値
int L=0;   //120°の時の値
int L1=0;  //100°の時の値

int q=0;   //状態関数　q=0->静止　q=1->直進
void move(int num_move){//停止0、前進 1、後退-1、右回転2、左回転-2
  if (num_move ==0){
    digitalWrite(RightP1, LOW);
    digitalWrite(LeftP1, LOW);
    digitalWrite(RightP2, LOW);
    digitalWrite(LeftP2, LOW);

  } else if(num_move ==-2){
    digitalWrite(RightP1, LOW);
    digitalWrite(LeftP1, HIGH);
    digitalWrite(RightP2, LOW);
    digitalWrite(LeftP2, HIGH);
    
  }else if(num_move ==2){
    digitalWrite(RightP1, HIGH);
    digitalWrite(LeftP1, LOW);
    digitalWrite(RightP2, HIGH);
    digitalWrite(LeftP2, LOW);
   
  }else if(num_move ==1){
    digitalWrite(RightP1, HIGH);
    digitalWrite(LeftP1, LOW);
    digitalWrite(RightP2, LOW);
    digitalWrite(LeftP2, HIGH);
   
  }else if(num_move ==-1){
    digitalWrite(RightP1, LOW);
    digitalWrite(LeftP1, HIGH);
    digitalWrite(RightP2, HIGH);
    digitalWrite(LeftP2, LOW);
    
  }
}
void setup() {
  Serial.begin( 9600 );
  myservo.attach(Shake_servoP);
  pinMode(LeftP1, OUTPUT);
  pinMode(RightP1, OUTPUT);
  pinMode(LeftP2, OUTPUT);
  pinMode(RightP2, OUTPUT);
  pinMode( Wall_Echo, INPUT );
  pinMode( Wall_Trig, OUTPUT );
  move(0);
}

void loop() {

  for (i=0; i<=8; i++){  //カウンタ関数を進めて配列に格納
  myservo.write(pos[i]);     //サーボを回転
  digitalWrite(Wall_Trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(Wall_Trig, HIGH ); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite(Wall_Trig, LOW );
  Duration = pulseIn(Wall_Echo, HIGH ); //センサからの入力
  if (Duration > 0) {
    Duration = Duration/2; //往復距離を半分にする
    Distance[i] = Duration*speed_of_sound*100/1000000; // 音速を340m/sに設定

    //評価する3つの値をそれぞれ格納
    N=Distance[5];
    R=Distance[1];
    L=Distance[9];
    R1=Distance[3];
    L1=Distance[7];

    if(i==0) delay(300);   //50°から130°までは距離があるため
    else delay(200);
}
  }
  
  switch(q){
    case 0 : 
    if(point<N&&point<R1&&point<L1){
    move(1);
    delay(500);
    q=1;  //状態
    delay(500);
  }

  else if(N<point&&L>R){
    q=2;
  }

  else if(N<point&&R>=L){
    q=5;
  }

  else{
    move(-1);
    delay(500);
    move(0);
  }
    break;

  case 1 : 
  if(point<N&&point<R1&&point<L1){
    move(1);
    delay(300);
  }

  else if(point>=N||point>=R1||point>=L1){
    move(0);
    q=0;
    delay(500);
  }
    break;

  case 2 :     //左回転
  
    move(-2);
    delay(1000);
    move(0);
    q=0;
    break;

  case 5 :   //右回転
 
    move(2);
    delay(1000);
    move(0);
    q=0;
    break;

  default :
    move(-1);
    delay(1000);
    move(0);
    q=0;
    break;
  }
}
