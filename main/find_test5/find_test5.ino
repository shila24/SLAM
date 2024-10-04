// 測距センサのピン番号
#include "Servo.h"
const int Wall_Echo = 12; // Echo Pin
const int Wall_Trig = 13; // Trigger Pin
const int Shake_servoP = 9; //首振りservo Pin
const int LeftP1   = 4;//RIN
const int RightP1  = 5;//FIN
const int LeftP2   = 2;//RIN
const int RightP2  = 3;//FIN
const int point = 20; //距離の基準 
const int point2 = 40; //距離の基準
const int point3 = 18;
double speed_of_sound = 331.5 + 0.6 * 20;
Servo myservo;
double Duration = 0; //受信した間隔
double Distance[11]; //距離 50°から130までの距離を10°ずつに9個の値を格納
double Distance2[12]; //距離 50°から130までの距離を10°ずつに9個の値を格納
unsigned long lastTime=0;
unsigned long preMillisTime = 0;
unsigned long startTime; 
int mode_check3 =0;//回る時0走る時1
int mode_check = 0;
int mode = 11;
bool motorRunning = false; // モーターが動作中かどうかを示すフラグ
int Count = 1;  //角度のカウント関数
int Count2 = 1;
int Count4 = 1;  //角度のカウント関数
int q=0;   //状態関数　q=0->静止　q=1->直進 q=5->左回転　q=2->右回転
int pos[11]={ 10,15,30,45,75,90,105,125,140,165,170 };  //サーボの角度　45°から135°までの11個値を格納
int N=0;   //90°の時の値/Users/akira/Documents/Arduino/waverun_test13/waverun_test13.ino
int R=0;   //63°の時の値
int R1=0;  //81°の時の値
int R2=0;  //72°の時の値
int L=0;   //117°の時の値
int L1=0;  //99°の時の値
int L2=0;  //108°の時の値

const int servoPin0 = 7; //腕部分右
const int servoPin1 = 8; //腕部分左
const int servoPinL = 6; //釣り上げ
//const int Wall_Echo = 12; // Echo Pin
//const int Wall_Trig = 13; // Trigger Pin
const int Object_Echo = 10; // Echo Pin
const int Object_Trig = 11; // Trigger Pin
//Servo myservo;
Servo mortor0;
Servo mortor1;
Servo mortorL;
//const int Shake_servoP = 9; //首振りservo Pin
//const int LeftP1   = 4;//RIN
//const int RightP1  = 5;//FIN
//const int LeftP2   = 2;//RIN
//const int RightP2  = 3;//FIN
//double speed_of_sound = 331.5 + 0.6 * 20;
//unsigned long lastTime=0;
//unsigned long preMillisTime = 0;
//unsigned long startTime; 
//const int point3 = 18;
int mode_check2 =0;//回る時0走る時1
//int mode_check = 0;
//bool motorRunning = false; // モーターが動作中かどうかを示すフラグ
//int q=0;   //状態関数　q=0->静止　q=1->直進 q=5->左回転　q=2->右回転
//int pos[11]={ 10,15,30,45,75,90,105,125,140,165,170 }; 
//int Count2 = 1;  //角度のカウント関数
//int N=0;   //90°の時の値/Users/akira/Documents/Arduino/waverun_test13/waverun_test13.ino
//int R=0;   //63°の時の値
//int R1=0;  //81°の時の値
//int R2=0;  //72°の時の値
//int L=0;   //117°の時の値
///int L1=0;  //99°の時の値
//int L2=0;  //108°の時の値
//double Duration = 0; //受信した間隔
//double Distance[11]; //距離 50°から130までの距離を10°ずつに9個の値を格納
//double Distance2[12]; //距離 50°から130までの距離を10°ずつに9個の値を格納
// 制御パラメータ
const int n = 10;       // センサの差がn以上の場合に停止する閾値
const int n1 = 20;      // センサの片方の値がpoint以上の場合の前進時間 (ミリ秒)
const int n2 = 30;      // センサの片方の値がpoint以下point2以上の場合の前進時間 (ミリ秒)
//const double point = 100;    // 前進する閾値
//const double point2 = 20;   // 後進する閾値
const double point_min = 3;
const double point_stop = 10;
const double point_stop2 = 4;
double firstsearch=222;
double firstmove=1000;
double secondsearch=40;
double secondmove = 100;
//int mode = 11;
//int mode_check3 = 0;
//int Count=1;
int Count3=1;
int memory_n;
int pos0 = 0; 
int pos1 = 0;
double duration = 100;
double distance = 100;
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

void distance_check(){
  myservo.write(pos[Count4]);     //サーボを回転
  digitalWrite(Wall_Trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(Wall_Trig, HIGH ); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite(Wall_Trig, LOW );
  Duration = pulseIn(Wall_Echo, HIGH ); //センサからの入力
  if (Duration > 0) {
    Duration = Duration/2; //往復距離を半分にする
    Distance[Count4] = Duration*speed_of_sound*100/1000000; // 音速を340m/sに設定
    //評価する5つの値をそれぞれ格納
    N=Distance[5];//90
    R=Distance[1];//54
    L=Distance[9];//126
    R1=Distance[3];//72
    L1=Distance[7];//108
    R2=Distance[0];//81
    L2=Distance[10];//99
    Count4=(Count4+1)%11;
  }
}

void distance_check2(){
  myservo.write(90);     //サーボを回転
  digitalWrite(Wall_Trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(Wall_Trig, HIGH ); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite(Wall_Trig, LOW );
  Duration = pulseIn(Wall_Echo, HIGH ); //センサからの入力
  if (Duration > 0) {
    Duration = Duration/2; //往復距離を半分にする
    Distance2[Count2] = Duration*speed_of_sound*100/1000000; // 音速を340m/sに設定
        //評価する7つの値をそれぞれ格納
    Count2 = (Count2+1)%12;
  }
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

int findMinIndex(double arr[], int size) {
    int minIndex = 0; // 最小値のインデックスを初期化
    double minValue = arr[0]; // 最小値を初期化

    // リストを走査して最小値とそのインデックスを見つける
    for (int i = 1; i < size; ++i) {
        if (arr[i] < minValue) {
            minValue = arr[i];
            minIndex = i;
        }
    }

    return minIndex; // 最小値のインデックスを返す
}

double findMinValue(double arr[], int size) {
  double minValue = arr[0]; // 最初の要素を最小値として初期化

  // リストを走査して最小値を見つける
  for (int i = 1; i < size; ++i) {
    if (arr[i] < minValue) {
      minValue = arr[i];
    }
  }

  return minValue; // 最小値を返す
}

void liftup(int t1,int t2){//巻き上げの秒数、保持の秒数
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

  
  
  
}

void wave_lift(int TRIG, int ECHO){
  
  if (duration >= 0) {
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
    distance = duration * speed_of_sound * 100 / 1000000;
    delay(1000);
    liftup(3000,4000);
  }
}


void setup() {
  // シリアル通信を開始
  Serial.begin(9600);
  // 測距センサのピンを設定
  myservo.attach(Shake_servoP);
  myservo.attach(Shake_servoP);
  mortor0.attach(servoPin0);
  mortor1.attach(servoPin1);
  mortorL.attach(servoPinL); 
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
  if(mode==0){//距離を取って遠いなら進む近いなら下がる  適切ならとまる
    Serial.print("A,");
    Serial.println(getDistance(Object_Trig, Object_Echo));
    if (getDistance(Object_Trig, Object_Echo)>point_stop){
      if (!motorRunning) {
        move(1);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime>=firstmove){
          move(0);
          motorRunning = false;
          mode =1;
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
          mode =1;
    
        }
      }
    }else if(getDistance(Object_Trig, Object_Echo)<point_stop&&getDistance(Object_Trig, Object_Echo)>point_min){
      move(0);
      mode =5;
    }
  }
  if(mode==1){ 
     if (!motorRunning) {
        move(-2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=firstsearch*5){
        move(0);
        mode = 2;
      }
    }
  }
  if(mode==2){//最初の値を記憶
    memory = getDistance(Object_Trig, Object_Echo);
    mode =3;
  }
  if(mode==3){//最小になる角度を取る
    if (!motorRunning) {
        move(2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=firstsearch){
        move(0);
        motorRunning = false;
        if (memory > getDistance(Object_Trig, Object_Echo)){
          memory  = getDistance(Object_Trig, Object_Echo);
          memory_n = Count;
        }
        Count =(Count+1)%10;
        if (Count == 0){
          mode =4;
        }
      }
    }
  }
  if(mode==4){ 
    if (!motorRunning) {//逆回転して最小値へ
      move(-2);
      startTime = millis(); 
      motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>= (10-memory_n)  * firstsearch){//ここの秒数を調節
        move(0);
        motorRunning = false;
        lastTime = millis();
        mode =0;
      }
    }
  }
  if(mode==5){//距離を取って遠いなら進む近いなら下がる  適切ならとまる
    Serial.print("B");
    Serial.println(getDistance(Object_Trig, Object_Echo));
    if (getDistance(Object_Trig, Object_Echo)>point_stop2){
      if (!motorRunning) {
        move(1);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime>=secondmove){
          move(0);
          motorRunning = false;
          mode =6;
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
          mode =6;
    
        }
      }
    }else if(getDistance(Object_Trig, Object_Echo)<point_stop2){
      move(0);
      mode = 10;
    }
  }
  if(mode==6){ 
     if (!motorRunning) {
        move(-2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=secondsearch*4.5){
        move(0);
        mode = 7;
      }
    }
  }
  if(mode==7){//最初の値を記憶
    memory = getDistance(Object_Trig, Object_Echo);
    mode =8;
  }
  if(mode==8){//最小になる角度を取る
    if (!motorRunning) {
        move(2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>=secondsearch){
        move(0);
        motorRunning = false;
        if (memory > getDistance(Object_Trig, Object_Echo)){
          memory  = getDistance(Object_Trig, Object_Echo);
          memory_n = Count;
        }
        Count =(Count+1)%10;
        if (Count == 0){
          mode =9;
        }
      }
    }
  }
  if(mode==9){ 
    if (!motorRunning) {//逆回転して最小値へ
      move(-2);
      startTime = millis(); 
      motorRunning = true;
    }else{
      currentMillis = millis();
      if(currentMillis- startTime>= (10-memory_n)  * secondsearch){//ここの秒数を調節
        move(0);
        motorRunning = false;
        lastTime = millis();
        mode =5;
      }
    }
  }
  if(mode==10){
    wave_lift(Object_Trig, Object_Echo);
    mode = 11;
  }
  if(mode==11){
    if (mode_check3 ==0){//一番近い壁の方向を探す
      switch(Count2){
        case 0 : 
          if (!motorRunning) {
            move(2);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=667){//ここの秒数を調節
              move(0);
              distance_check2();
              motorRunning = false;
              mode_check3 =1;
              }
            }  
          break;  
        default:
          if (!motorRunning) {
            move(2);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=667){//ここの秒数を調節
              move(0);
              distance_check2();
              motorRunning = false;
            }
          }
          break;
      }
    }

    if (mode_check3 ==1){ 
      if (!motorRunning) {//一番近い壁の方向を向く
        move(2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime >= ((findMinIndex(Distance2, 12) + 9)%12+0.1) * 667){//ここの秒数を調節
          move(0);
          motorRunning = false;
          lastTime = millis();
          mode_check3 =2;
        }
      }
    
    }

    if(mode_check3 == 2){
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
        }
      }
      
      if (mode_check ==1){ 
        switch(q){
          case 0 : 
            if(point<N && point<R1 && point<L1 && point < R && point < L){
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
            else if(N==0 || R==0 || L==0 || R1==0 || L1==0){
              q=1;
            }
            else{
              if (!motorRunning) {
                move(0);
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
            break;

          case 1 : //前進
            if(point<N && point<R1 && point<L1&&point<L&&point<R){
              if (!motorRunning) {
                move(1);
                startTime = millis(); // 動作開始時間を記録
                motorRunning = true;
              }else{
                currentMillis = millis();
                if(currentMillis- startTime >= 1000*findMinValue(Distance2, 12)*0.8 / 10){ // 距離から直線距離で進んでいい距離（時間）を割り出して時間短縮したい
                  move(0);
                  q = 2;
                  mode_check =0;
                  motorRunning = false;
                }
              }
            }
            else{
              mode_check3 = 3;
            }         
            break;

          case 2 : //前進2
            if(point3<N && point3<R1 && point3<L1 && point3 < L && point3<R){
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
              }
            }
            else{
              mode_check3 = 3;
            }         
            break;
        }
      }
    }

    if (mode_check3 == 3){//一番近い壁の方向を探す
      switch(Count2){
        case 0 : 
          if (!motorRunning) {
            move(-2);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=667){//ここの秒数を調節
              move(0);
              distance_check2();
              motorRunning = false;
              myservo.write(90);
              mode_check3 =4;
              }
            }  
          break;  
        default:
          if (!motorRunning) {
            move(-2);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=667){//ここの秒数を調節
              move(0);
              distance_check2();
              motorRunning = false;
            }
          }
          break;
      }
    }

    if (mode_check3 ==4){ 
      if (!motorRunning) {//一番近い壁の方向を向く
        move(-2);
        startTime = millis(); // 動作開始時間を記録
        motorRunning = true;
      }else{
        currentMillis = millis();
        if(currentMillis- startTime >= ((findMinIndex(Distance2, 12) + 6)%12+0.8) * 667-667*11){//ここの秒数を調節
          move(0);
          motorRunning = false;
          lastTime = millis();
          mode_check3 = 5;
        }
      }
    }

    if(mode_check3 == 5){
      move(0);
      Serial.print("Done");
    }
  }
}


