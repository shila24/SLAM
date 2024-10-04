#ifndef Rolllift_h
#define Rolllift_h
#include "arduino.h"
#include "servo.h"

class Roll_Lift
{
public:
  Roll_Lift(int Roll_Pin1);   // コンストラクタ
  void Rollup();          // リフト上昇
  void 
  void Rolldown();         // リフト下降

private:
  int Roll_Pin1;      // クラス内で使用するメンバ変数

};

#endif
