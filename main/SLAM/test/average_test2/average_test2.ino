#define TRIG_PIN 2
#define ECHO_PIN 3

const int numReadings = 5;  // 移動平均を取るための読み取り数
int readings[numReadings];   // 距離の読み取り値を格納する配列
int index = 0;               // readings 配列のインデックス
int total = 0;               // readings 配列の合計
int average = 0;             // 移動平均の値
int minDistance = 9999;      // 最小の距離を格納する変数

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // 測距センサからの距離を取得
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // 距離を移動平均に追加
  total -= readings[index];
  readings[index] = distance;
  total += distance;
  index = (index + 1) % numReadings;
  average = total / numReadings;

  // 最小の距離を更新
  if (average < minDistance) {
    minDistance = average;
  }

  // 0.5秒ごとにシリアルモニタに距離と最小距離を表示
  static unsigned long previousMillis = 0;
  const unsigned long interval = 500;  // 0.5秒ごと
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Distance: ");
    Serial.print(average);
    Serial.print(" cm");
    Serial.print(" Min Distance: ");
    Serial.print(minDistance);
    Serial.println(" cm");
  }
}