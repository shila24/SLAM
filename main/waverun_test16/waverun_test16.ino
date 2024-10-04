// 測距センサのピン番号
#include "Servo.h"
#include <Wire.h>
#define S11059_ADDR 0x2A
#define CONTROL_MSB 0x00
#define CONTROL_1_LSB 0x89
#define CONTROL_2_LSB 0x09
#define SENSOR_REGISTER 0x03

const int LeftP2   = 2;//RIN
const int RightP2  = 3;//FIN
const int LeftP1   = 4;//RIN
const int RightP1  = 5;//FIN
const int servoPinL = 6; //釣り上げ
const int servoPin0 = 7; //腕部分右
const int servoPin1 = 8; //腕部分左
const int Shake_servoP = 9; //首振りservo Pin
const int Object_Echo = 10; // Echo Pin
const int Object_Trig = 11; // Trigger Pin
const int Wall_Echo = 12; // Echo Pin
const int Wall_Trig = 13; // Trigger Pin
const int LED = 14;//LED白
const int LED_red = 15;//LED赤
const int LED_green = 16;//LED緑

//Servomotorの宣言
Servo myservo;
Servo mortor0;
Servo mortor1;
Servo mortorL;
//音速などの制御パラメータ
double speed_of_sound = 331.5 + 0.6 * 20;//音速
const int n = 10;       // センサの差がn以上の場合に停止する閾値
const int n1 = 20;      // センサの片方の値がpoint以上の場合の前進時間 (ミリ秒)
const int n2 = 30;      // センサの片方の値がpoint以下point2以上の場合の前進時間 (ミリ秒)
const int point = 20; //距離の基準 
const double point1 = 100;    // 前進する閾値
const double point2 = 20;   // 後進する閾値
const double point_min = 3;//物体の前で止まる位置(近すぎる)
const double point_stop = 10;//物体の前で止まるシステムに切り替え
const double point_stop2 = 4;//物体の前で止まる位置
unsigned long lastTime=0;//
unsigned long startTime; 
unsigned long preMillisTime = 0;
double currentMillis = 0;
double memory = 0;
double Duration = 0; //受信した間隔
double distance = 100;
double Distance[11]; //距離 50°から130までの距離を10°ずつに9個の値を格納
double Distance3[24]; //360回転して物体を見つける
int N=0;   //90°の時の値
int R=0;   //63°の時の値
int R1=0;  //81°の時の値
int R2=0;  //72°の時の値
int L=0;   //117°の時の値
int L1=0;  //99°の時の値
int L2=0;  //108°の時の値
int pos0 = 0;//アーム部分のモーター1の角度指定
int pos1 = 0;//アーム部分のモーター2の角度指定
const double out = 500;//外出る
//リスト
int pos[11]={ 10,15,30,45,75,90,105,125,140,165,170 };  //サーボの角度　45°から135°までの11個値を格納

//count系統

//フラグ
int q=0;   //状態関数　q=0->静止　q=1->直進 q=5->左回転　q=2->右回転
int run_mode = 0;
int mode =0;
int mode_check =0;//サーボ撮る時0走る時1
int mode_check2 =0;//サーボ撮る時0走る時1
int Count=1;
int Count2=1;
int Count3=1;
bool motorRunning = false; // モーターが動作中かどうかを示すフラグ
int memory_n;

void setup() {
  Serial.begin( 9600 );
  myservo.attach(Shake_servoP);
  pinMode(LeftP1, OUTPUT);
  pinMode(RightP1, OUTPUT);
  pinMode(LeftP2, OUTPUT);
  pinMode(RightP2, OUTPUT);
  pinMode( Wall_Echo, INPUT );
  pinMode( Wall_Trig, OUTPUT );
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

void distance_check(){
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
  R2=Distance[0];//81
  L2=Distance[10];//99
  Count=(Count+1)%11;
  
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (mode_check ==0){
    switch(Count){
      case 0 : 
        if (currentMillis - lastTime > 300) {
          lastTime = millis();
          distance_check();
          mode_check =1;
          preMillisTime = millis();          
        }
        break;
        
      default :
        if (currentMillis - lastTime > 50) {
          distance_check();
          lastTime = millis();
        }  
        break;
    }}
  
  if (mode_check ==1){  
    Serial.print("1:");
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
    Serial.print("L2:");
    Serial.println(L2);
    Serial.print("R2:");
    Serial.println(R2);

    switch(q){
      case 0 : 
        if(point<N && point<R1 && point<L1 && point < R && point < L&&(R2<out||L2<out)){
          if (!motorRunning) {
            move(1);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=1000){
              move(0);
              q=1;  //状態
              lastTime = millis();
              mode_check =0;
              motorRunning = false;
            }
          }
        }
        else if(N>0 && N<point && R>L){
          q=2;
        }
        else if(N>0 && N<point && L>=R){
          q=5;
        }
        else if(L1>0 && L1<15 && R1>15 && N>point){
          q=2;
        }
        else if(R1>0 && R1<15 && L1>15 && N>point){
          q=5;
        }
        else if(N>point && R1>point && L1>point && L<point){
          q=3;
        }
        else if(N>point && R1>point && L1>point && R<point){
          q=6;
        }
        else if(N==0 || R==0 || L==0 || R1==0 || L1==0){
          q=1;
        }
        else if(R2>out&&L2>out){
          q=7;
        }
        else{
          if (!motorRunning) {
            move(1);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=300){
              move(0);
              q=1;  //状態
              lastTime = millis();
              mode_check =0;
              motorRunning = false;
            }
          }
        }
        break;
      case 1 : 

        if(point<N && point<R1 && point<L1&&point<L&&point<R&&(R2<out||L2<out)){
          if (!motorRunning) {
            move(1);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=1000){
              move(0);

              mode_check =0;
              motorRunning = false;
            }
          }}
        else{  
          if (!motorRunning) {
            move(0);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=1000){
              q = 0;
              mode_check =0;
              motorRunning = false;
            }
          }
        }         
        break;

      case 2 :     //右回転
        if (!motorRunning) {
          move(2);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=1000){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =0;
            motorRunning = false;
            }
          }
        break;
      case 3 :     //右小回転
        if (!motorRunning) {
          move(2);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=500){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =0;
            motorRunning = false;
            }
          }
        break;

      case 5 :   //左回転
        if (!motorRunning) {
          move(-2);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=1000){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =0;
            motorRunning = false;
            }
          }
      case 6 :   //左小回転
        if (!motorRunning) {
          move(-2);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=500){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =0;
            motorRunning = false;
            }
          }    
        break;
      case 7 :   //左小回転
        move(0);   
        break;

      default :
        if (!motorRunning) {
          move(-1);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=500){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =0;
            motorRunning = false;
            }
          }
        break;
    }
  
  }
}


