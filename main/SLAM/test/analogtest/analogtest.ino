int motorPin1 = 9; // PWMピン1
int motorPin2 = 10; // PWMピン2

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
}

void loop() {
  // モーターを正転（前進）させる
  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(motorPin1, speed);
    analogWrite(motorPin2, 0);
    delay(10); // 速度を変更する時間間隔
  }

  delay(1000); // 方向転換までの待ち時間

  // モーターを逆転（後進）させる
  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, speed);
    delay(10); // 速度を変更する時間間隔
  }

  delay(1000); // 方向転換までの待ち時間
}
