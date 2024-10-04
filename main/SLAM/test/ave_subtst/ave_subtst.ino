#define Wall_TRIG 13
#define Wall_ECHO 12
#define Target_TRIG 11
#define Target_ECHO 10

const int numReadings = 5;  // 移動平均を取るための読み取り数
double readings[numReadings] ={0.0, 0.0, 0.0, 0.0, 0.0 };   // 距離の読み取り値を格納する配列
int index = 0;               // readings 配列のインデックス
double total = 0.0;               // readings 配列の合計
double average = 0.0;             // 移動平均の値
double minDistance = 9999.0;      // 最小の距離を格納する変数
double value[4] = {0.0,0.0,0.0,0.0};


void setup() {
  Serial.begin(9600);
  pinMode(Wall_TRIG, OUTPUT);
  pinMode(Wall_ECHO, INPUT);
  pinMode(Target_TRIG, OUTPUT);
  pinMode(Target_ECHO, INPUT);
}

void loop() {
  measureDistance(Wall_TRIG,Wall_ECHO,0,1);
  measureDistance(Target_TRIG,Target_ECHO,2,3);
  
  displayDistance(value[0],value[1],value[2],value[3]);
}

void measureDistance(int TRIG_PIN,int ECHO_PIN,int n1,int n2) {
  // 測距センサからの距離を取得
  double duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.034) / 2;

  // 距離を移動平均に追加
  total += distance;
  total -= readings[index];
  readings[index] = distance;
  
  index = (index + 1) % numReadings;
  delay(5);
  average = total / numReadings;
  if (average < minDistance&&checkNonZero(readings,numReadings)==1) {
    minDistance = average;
  }
  value[n1] = average;
  value[n2] = minDistance;
  
}



int checkNonZero(double reading[], int length) {
    for (int i = 0; i < length; i++) {
        if (reading[i] == 0.0) {
            return 0; // 要素に0がある場合は0を返す
        }
    }
    return 1; // 要素に0がない場合は1を返す
}



void displayDistance(double average1,double minDistance1,double average2,double minDistance2) {
  // 0.5秒ごとにシリアルモニタに距離と最小距離を表示
  static unsigned long previousMillis = 0;
  const unsigned long interval = 500;  // 0.5秒ごと
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Distance: ");
    Serial.print(average1);
    Serial.print(" cm");
    Serial.print("Distance: ");
    Serial.print(average2);
    Serial.print(" cm");
    
    Serial.print(" Min Distance: ");
    Serial.print(minDistance1);
    Serial.print(" cm");
    Serial.print(" Min Distance: ");
    Serial.print(minDistance2);
    Serial.println(" cm");
    
  }
}