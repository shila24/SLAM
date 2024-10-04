#include "Arduino.h"
#include "Servo.h"
#include "Rolllift.h"

Rolllift::Rolllift(int pin) //コンストラクタ
{
     pinMode(pin, OUTPUT) ;        // 指定されたデジタルピン番号をＬＥＤ出力に設定
     Roll_Pin1 = pin ;
}

void Rolllift::Rollup();//リフトの巻き上げ
{
     digitalWrite(Roll_Pin1, HIGH) ; // LEDを点灯する
}
// リフトの下降
void LightingControl::Off()
{
     digitalWrite(LED_PinNo, LOW) ;  // LEDを消灯する
}