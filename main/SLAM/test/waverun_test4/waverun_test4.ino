#include "Servo.h"
const int Wall_Echo = 12; // Echo Pin
const int Wall_Trig = 13; // Trigger Pin
const int Shake_servoP = 9; //首振りservo Pin
const int LeftP1   = 4;//RIN
const int RightP1  = 5;//FIN
const int LeftP2   = 2;//RIN
const int RightP2  = 3;//FIN
const int point = 30; //距離の基準 
double speed_of_sound = 331.5 + 0.6 * 25;
Servo myservo;
double Duration = 0; //受信した間隔
double Distance[11]; //距離 50°から130までの距離を10°ずつに9個の値を格納
unsigned long lastTime=0;

int pos[11]={45,54,63,72,81,90,99,108,117,126,135};  //サーボの角度　45°から135°までの11個値を格納

int Count = 0;  //角度のカウント関数
int N=0;   //90°の時の値
int R=0;   //63°の時の値
int R1=0;  //81°の時の値
int R2=0;  //72°の時の値
int L=0;   //117°の時の値
int L1=0;  //99°の時の値
int L2=0;  //108°の時の値

int q=0;   //状態関数　q=0->静止　q=1->直進 q=5->左回転　q=2->右回転
unsigned long previousMillis = 0;  // 最後にアクションが実行された時間
const long interval = 1000; 

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
  switch(Count){
    case 0 : 
    if ((millis() - lastTime) > 300) {
      disrance_check();
      }
    break;
      
    default :
    if ((millis() - lastTime) > 50) {
        disrance_check();
      }  
    break;
  }
    
  switch(q){
    case 0 : 
    if(point<N&&point<R1&&point<L1){
      move(1);
      q=1;  //状態
      delay(500);
    }

    else if(N>0&&N<point&&R>L){
      q=2;
    }
 
    else if(N>0&&N<point&&L>=R){
      q=5;
    }
    else if(L1>0&&L1<point&&R1>L1){
      q=2;
    } 
    else if(R1>0&&R1<point&&R1<L1){
      q=5;
    }
    else if(N==0||R==0||L==0||R1==0||L1==0){
      q=1;
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

    case 2 :     //右回転
    move(2);
    delay(200);
    move(0);
    q=0;
    break;

    case 5 :   //左回転
    move(-2);
    delay(200);
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
void disrance_check(){
  lastTime = millis();
  myservo.write(pos[Count]);     //サーボを回転
  digitalWrite(Wall_Trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(Wall_Trig, HIGH ); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite(Wall_Trig, LOW );
  Duration = pulseIn(Wall_Echo, HIGH ); //センサからの入力
  if (Duration > 0) {
  Duration = Duration/2; //往復距離を半分にする
  Distance[Count] = Duration*speed_of_sound*100/1000000; // 音速を340m/sに設定
  //評価する5つの値をそれぞれ格納
  N=Distance[5];//90
  R=Distance[1];//54
  L=Distance[9];//126
  R1=Distance[3];//72
  L1=Distance[7];//108
  R2=Distance[4];//81
  L2=Distance[6];//99
  Count=(Count+1)%11;
  Serial.print(Count);
  Serial.print("N:");
  Serial.println(N);
  Serial.print("L1:");
  Serial.println(L1);
  Serial.print("L:");
  Serial.println(L);
  Serial.print("R1:");
  Serial.println(R1);
  Serial.print("R:");
  Serial.println(R);
  }
}
