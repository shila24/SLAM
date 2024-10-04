// 定義
#define trigPin 13
#define echoPin 12

void setup() {
  Serial.begin(9600); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
}

void loop() {
  long duration, distance; 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  // 音波が物体に達するまでの時間を距離に変換
  distance = duration * 0.034 / 2; // 音速(約34 cm/ms)で割り、往復距離なので2で割る

  // 測定結果をシリアルモニターに出力
  Serial.print("Distance: ");
  Serial.println(distance);

  delay(100); // 測定を安定化するための遅延
}