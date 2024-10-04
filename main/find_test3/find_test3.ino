// 測距センサのピン番号
#include "Servo.h"
const int servoPin0 = 7; //腕部分右
const int servoPin1 = 8; //腕部分左
const int servoPinL = 6; //釣り上げ
const int Wall_Echo = 12; // Echo Pin
const int Wall_Trig = 13; // Trigger Pin
const int Object_Echo = 10; // Echo Pin
const int Object_Trig = 11; // Trigger Pin
Servo myservo;
Servo mortor0;
Servo mortor1;
Servo mortorL;
const int Shake_servoP = 9; //首振りservo Pin
const int LeftP1   = 4;//RIN
const int RightP1  = 5;//FIN
const int LeftP2   = 2;//RIN
const int RightP2  = 3;//FIN
double speed_of_sound = 331.5 + 0.6 * 20;
unsigned long lastTime=0;
unsigned long preMillisTime = 0;
unsigned long startTime; 
int mode_check2 =0;//回る時0走る時1
bool motorRunning = false; // モーターが動作中かどうかを示すフラグ
// 制御パラメータ
const int n = 10;       // センサの差がn以上の場合に停止する閾値
const int n1 = 20;      // センサの片方の値がpoint以上の場合の前進時間 (ミリ秒)
const int n2 = 30;      // センサの片方の値がpoint以下point2以上の場合の前進時間 (ミリ秒)
const double point = 100;    // 前進する閾値
const double point2 = 20;   // 後進する閾値
const double point_min = 3;
const double point_stop = 10;
const double point_stop2 = 4;
int mode =0;
int Count=1;
int Count3=1;
int memory_n;
double currentMillis = 0;
double memory = 0;
double Distance3[24];
double getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}
void distance_check3(){
  double difference1 = getDistance(Object_Trig, Object_Echo);
  double difference2 = getDistance(Wall_Trig, Wall_Echo); 
  Distance3[Count3] = difference2 - difference1; // 音速を340m/sに設定
  //評価する7つの値をそれぞれ格納
  Serial.print("W");
  Serial.print(getDistance(Wall_Trig, Wall_Echo));
  Serial.print("O");
  Serial.print(getDistance(Object_Trig, Object_Echo));
  Count3 = (Count3+1)%24;
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
int findMaxIndex(double arr[], int size) {
    int maxIndex = 0; // 最小値のインデックスを初期化
    double maxValue = arr[0]; // 最小値を初期化

    // リストを走査して最小値とそのインデックスを見つける
    for (int i = 1; i < size; ++i) {
        if (arr[i] > maxValue) {
            maxValue = arr[i];
            maxIndex = i;
        }
    }

    return maxIndex; // 最小値のインデックスを返す
}
void setup() {
  // シリアル通信を開始
  Serial.begin(9600);
  // 測距センサのピンを設定
  Serial.begin( 9600 );
  myservo.attach(Shake_servoP);
  pinMode(LeftP1, OUTPUT);
  pinMode(RightP1, OUTPUT);
  pinMode(LeftP2, OUTPUT);
  pinMode(RightP2, OUTPUT);
  pinMode( Wall_Echo, INPUT );
  pinMode( Wall_Trig, OUTPUT );
  pinMode( Object_Echo, INPUT );
  pinMode( Object_Trig, OUTPUT );
}

void loop() {
  currentMillis = millis();
  myservo.write(90); 
  if(mode==0){//遠い間は大回転して近い方へ
    if (getDistance(Object_Trig, Object_Echo)<point2&&getDistance(Wall_Trig, Wall_Echo) > point2+3){//壁じゃない
      mode = 3;
    }else {
      if (!motorRunning) {
        move(2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime>=330){
          move(0);
          distance_check3();
          motorRunning = false;
          if (Count3 ==0){
            mode=1;

          }
        }
      }
    }
  }
  if (mode ==1){ 
    if (!motorRunning) {//一番近い壁の方向を向く
      move(2);
      startTime = millis(); // 動作開始時間を記録
      motorRunning = true;
    }else{
      currentMillis = millis();
      int a = findMaxIndex(Distance3, 24); 
      Serial.println(a);
      if(currentMillis- startTime>= a * 330){//ここの秒数を調節
        move(0);
        motorRunning = false;
        lastTime = millis();
        mode =2;
      }
    }
  }
  if(mode==2){//本体を大雑把に進める
    if (!motorRunning) {
      move(1);
      startTime = millis(); // 動作開始時間を記録
      motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=3000){
        move(0);
        motorRunning = false;
        mode =0;
      }
    }
  }
  if(mode==3){//距離を取って遠いなら進む近いなら下がる  適切ならとまる
    Serial.print("A,");
    Serial.println(getDistance(Object_Trig, Object_Echo));
    if (getDistance(Object_Trig, Object_Echo)>point_stop){
      if (!motorRunning) {
        move(1);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime>=2000){
          move(0);
          motorRunning = false;
          mode =4;
        }
      }
    }else if(getDistance(Object_Trig, Object_Echo)<point_min){
      if (!motorRunning) {
        move(-1);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime>=50){
          move(0);
          motorRunning = false;
          mode =4;
    
        }
      }
    }else if(getDistance(Object_Trig, Object_Echo)<point_stop&&getDistance(Object_Trig, Object_Echo)>point_min){
      move(0);
      mode =8;
    }
  }
  if(mode==4){//最初の値を記憶
    memory = getDistance(Object_Trig, Object_Echo);
    mode =7;
  }
  if(mode==5){//最小になる角度を取る
    if (!motorRunning) {
        move(2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=222){
        move(0);
        motorRunning = false;
        if (memory > getDistance(Object_Trig, Object_Echo)){
          memory  = getDistance(Object_Trig, Object_Echo);
          memory_n = Count;
        }
        Count =(Count+1)%9;
        if (Count == 0){
          mode =6;
        }
      }
    }
  }
  if(mode==6){ 
    if (!motorRunning) {//逆回転して最小値へ
      move(-2);
      startTime = millis(); 
      motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>= (9-memory_n)  * 200){//ここの秒数を調節
        move(0);
        motorRunning = false;
        lastTime = millis();
        mode =3;
      }
    }
  }
  if(mode==7){ 
     if (!motorRunning) {
        move(-2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=888){
        move(0);
        mode = 5;
      }
    }
  }
  if(mode==8){//距離を取って遠いなら進む近いなら下がる  適切ならとまる
    Serial.print("B,");
    Serial.println(getDistance(Object_Trig, Object_Echo));
    if (getDistance(Object_Trig, Object_Echo)>point_stop2){
      if (!motorRunning) {
        move(1);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime>=200){
          move(0);
          motorRunning = false;
          mode =9;
        }
      }
    }else if(getDistance(Object_Trig, Object_Echo)<point_min){
      if (!motorRunning) {
        move(-1);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime>=50){
          move(0);
          motorRunning = false;
          mode =9;
    
        }
      }
    }else if(getDistance(Object_Trig, Object_Echo)<point_stop2&&getDistance(Object_Trig, Object_Echo)>point_min){
      move(0);

    }
  }
  if(mode==9){//最初の値を記憶
    memory = getDistance(Object_Trig, Object_Echo);
    mode =12;
  }
  if(mode==10){//最小になる角度を取る
    if (!motorRunning) {
        move(2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=100){
        move(0);
        motorRunning = false;
        if (memory > getDistance(Object_Trig, Object_Echo)){
          memory  = getDistance(Object_Trig, Object_Echo);
          memory_n = Count;
        }
        Count =(Count+1)%10;
        if (Count == 0){
          mode =11;
        }
      }
    }
  }
  if(mode==11){ 
    if (!motorRunning) {//逆回転して最小値へ
      move(-2);
      startTime = millis(); 
      motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>= (11-memory_n)  * 60){//ここの秒数を調節
        move(0);
        motorRunning = false;
        lastTime = millis();
        mode =8;
      }
    }
  }
  if(mode==12){
    Serial.println();
     if (!motorRunning) {
        move(-2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=500){
        move(0);
        mode = 10;
      }
    }
  
  }
}

