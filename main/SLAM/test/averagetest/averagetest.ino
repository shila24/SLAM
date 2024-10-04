#define SENSOR_PIN A0
#define NUM_SAMPLES 10 // 移動平均を計算するためのサンプル数

int samples[NUM_SAMPLES]; // 測距センサのサンプル値を保存する配列
int currentIndex = 0; // 現在のサンプルのインデックス

void setup() {
  Serial.begin(9600); // シリアルモニタを開く
}

void loop() {
  // センサから距離を取得
  int sensorValue = analogRead(SENSOR_PIN);

  // 配列にサンプルを追加
  samples[currentIndex] = sensorValue;
  currentIndex = (currentIndex + 1) % NUM_SAMPLES; // 次のインデックスを更新

  // 移動平均を計算
  int sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += samples[i];
  }
  float movingAverage = sum / (float)NUM_SAMPLES; // キャストを使用して浮動小数点数に変換

  // 移動平均をシリアルモニタに出力
  Serial.print("Moving Average: ");
  Serial.println(movingAverage);

  delay(100); // サンプリングレートを制御するための適切な遅延を追加
}