#include "Servo.h"
const int Wall_Echo = 12; // Echo Pin
const int Wall_Trig = 13; // Trigger Pin
const int Shake_servoP = 9; //首振りservo Pin
const int LeftP1   = 4;//RIN
const int RightP1  = 5;//FIN
const int LeftP2   = 2;//RIN
const int RightP2  = 3;//FIN
const int point = 30; //距離の基準 
const int point2 = 40; //距離の基準
double speed_of_sound = 331.5 + 0.6 * 20;
Servo myservo;
double Duration = 0; //受信した間隔
double Distance[11]; //距離 50°から130までの距離を10°ずつに9個の値を格納
double Distance2[12]; //距離 50°から130までの距離を10°ずつに9個の値を格納
unsigned long lastTime=0;
unsigned long preMillisTime = 0;
unsigned long startTime; 
int mode_check2 =0;//回る時0走る時1
bool motorRunning = false; // モーターが動作中かどうかを示すフラグ
int Count = 1;  //角度のカウント関数
int Count2 = 1;  //角度のカウント関数
int q=0;   //状態関数　q=0->静止　q=1->直進 q=5->左回転　q=2->右回転
int pos[11]={ 20,35,50,60,75,90,105,120,130,145,160 };  //サーボの角度　45°から135°までの11個値を格納

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
bool allValuesOverN(int values[], int size, int n) {//全ての値がn以上だとTrue
  for (int i = 0; i < size; ++i) {
    if (values[i] < n) {
      return false;
    }
  }
  return true;
}
bool check3distance(int val1, int val2, int val3, int n) {
    uint8_t count = 0; // uint8_tを使用してメモリ使用量を最小限に抑える

    if (val1 <= n) {
        count++;
    }
    if (val2 <= n) {
        count++;
    }
    if (val3 <= n) {
        count++;
    }

    return count >= 2;
}
int findMinIndex(int arr[], int size) {
    int minIndex = 0; // 最小値のインデックスを初期化
    int minValue = arr[0]; // 最小値を初期化

    // リストを走査して最小値とそのインデックスを見つける
    for (int i = 1; i < size; ++i) {
        if (arr[i] < minValue) {
            minValue = arr[i];
            minIndex = i;
        }
    }

    return minIndex; // 最小値のインデックスを返す
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
  Count2 = (Count+1)%12;

  
  }
}
void loop() {  
  unsigned long currentMillis = millis();
  if (mode_check2 ==0){//一番近い壁の方向を探す
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
            mode_check =1;
            }
          }    
      default :
        if (!motorRunning) {
          move(2);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=667){//ここの秒数を調節
            move(0);
            distance_check2();
            motorRunning = false;}}
      break;
    }}
  
  if (mode_check2 ==1){  
    if (!motorRunning) {//一番近い壁の方向を向く
      move(2);
      startTime = millis(); // 動作開始時間を記録
      motorRunning = true;
    }else{
      currentMillis = millis();
      a = findMinIndex(Distance2[12], 12); 
      if(currentMillis- startTime>= a*667){//ここの秒数を調節
        move(0);
        motorRunning = false;
        lastTime = millis();
        mode_check ==2;}}}
  if (mode_check2 ==2){//首振りしながら壁まで近づく(首振り)
    switch(Count){
      case 0 : 
        if (currentMillis - lastTime > 300) {
          lastTime = millis();
          distance_check();
          mode_check =2;
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
  if (mode_check2 ==3){ //壁に近づき、通路側を向く
    values[5] =  {N, L1, R1, L, R};
    switch(q){
      case 0 ://ニュートラル 
        if (N>point2&&L1>point2&&R1>point2&&L>point2&&R>point2){//遠い
          q = 1;
        }
        else if (L1<point2){//遠い
          q = 1;
        }
        else if(R<point2&&R>point&&R<=L){//Rが1番近い
          q = 1;
        }
        else if(R<point&&R<=L){
          q = 6;
        }
        else if(L<point2&&L>point&&L<R){//Lが1番近い
          q = 1;
        }
        else if(L<point&&R<=L){
          q = 3
        }
        else if(check3distance(N, L1, R1,point)&&R<point2){
          q = 4;
        }
        else if(check3distance(N, L1, R1,point)&&L<point2){
          q = 4;
        }
        else if(allValuesOverN(values[5], 5, point2)){
          q = 1;
        }
        else if(N>0 && N<point2 && R>L){
          q=2;
        }
        else if(N>0 && N<point2 && L>=R){
          q=5;
        }
        else if(L1>0 && L1<point && R1>point && N>point){
          q=2;
        }
        else if(R1>0 && R1<point && L1>point && N>point){
          q=5;
        }
        else if(N>point && R1>point && L1>point && L<point){
          q=2;
        }
        else if(N>point && R1>point && L1>point && R<point){
          q=5;
        }
        else if(N==0 || R==0 || L==0 || R1==0 || L1==0){
          q=1;
        }
        else{
          if (!motorRunning) {
            move(-1);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=1000){
              move(0);
              q=1;  //状態
              lastTime = millis();
              mode_check =2;
              motorRunning = false;
            }
          }
        }
        break;

      case 1 : //前進
        if (!motorRunning) {
          move(1);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=1000){
            move(0);
            mode_check =2;
            motorRunning = false;
            }
          }
      break;

      case 4 : //停止 
        if (!motorRunning) {
          move(0);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=500){
            q = 0;
            mode_check =2;
            motorRunning = false;
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
          if(currentMillis- startTime>=200){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =2;
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
            mode_check =2;
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
          if(currentMillis- startTime>=200){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =2;
            motorRunning = false;
            }
          }    
        break;

      default :
        if (!motorRunning) {
          move(-1);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=1000){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =2;
            motorRunning = false;
            }
          }
        break;
    }
  
    }
  if (mode_check2 ==4){ //壁に沿って動く
    values[5] =  {N, L1, R1, L, R};
    switch(q){
      case 0 ://ニュートラル 
        if (N<point||)
        else if(R<point2&&R>point&&R<=L){//Rが1番近い
          q = 1
        }
        else if(R<point&&R<=L){
          q = 6
        }
        else if(L<point2&&L>point&&L<R){//Lが1番近い
          q = 1
        }
        else if(L<point&&R<=L){
          q = 3
        }
        else if(check3distance(N, L1, R1,point)&&R<point2){
          q = 4;
        }
        else if(check3distance(N, L1, R1,point)&&L<point2){
          q = 4;
        }
        else if(allValuesOverN(values[5], 5, point2)){
          q = 1;
        }
        else if(N>0 && N<point2 && R>L){
          q=2;
        }
        else if(N>0 && N<point2 && L>=R){
          q=5;
        }
        else if(L1>0 && L1<point && R1>point && N>point){
          q=2;
        }
        else if(R1>0 && R1<point && L1>point && N>point){
          q=5;
        }
        else if(N>point && R1>point && L1>point && L<point){
          q=2;
        }
        else if(N>point && R1>point && L1>point && R<point){
          q=5;
        }
        else if(N==0 || R==0 || L==0 || R1==0 || L1==0){
          q=1;
        }
        else{
          if (!motorRunning) {
            move(-1);
            startTime = millis(); // 動作開始時間を記録
            motorRunning = true;
          }else{
            currentMillis = millis();
            if(currentMillis- startTime>=1000){
              move(0);
              q=1;  //状態
              lastTime = millis();
              mode_check =2;
              motorRunning = false;
            }
          }
        }
        break;

      case 1 : //前進
        if (!motorRunning) {
          move(1);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=1000){
            move(0);
            mode_check =2;
            motorRunning = false;
            }
          }
      break;

      case 4 : //停止 
        if (!motorRunning) {
          move(0);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=500){
            q = 0;
            mode_check =2;
            motorRunning = false;
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
          if(currentMillis- startTime>=200){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =2;
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
            mode_check =2;
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
          if(currentMillis- startTime>=200){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =2;
            motorRunning = false;
            }
          }    
        break;

      default :
        if (!motorRunning) {
          move(-1);
          startTime = millis(); // 動作開始時間を記録
          motorRunning = true;
        }else{
          currentMillis = millis();
          if(currentMillis- startTime>=1000){
            move(0);
            q=0;  //状態
            lastTime = millis();
            mode_check =2;
            motorRunning = false;
            }
          }
        break;
    
  }

