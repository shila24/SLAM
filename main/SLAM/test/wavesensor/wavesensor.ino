#include "Servo.h"
const int Wall_Echo = 12; // Echo Pin
const int Wall_Trig = 13; // Trigger Pin
const int Object_Echo = 10; // Echo Pin
const int Object_Trig = 11; // Trigger Pin
const int Shake_servoP = 9; //首振りservo Pin
const int LeftP1   = 4;//RIN
const int RightP1  = 5;//FIN
const int LeftP2   = 2;//RIN
const int RightP2  = 3;//FIN
double total= 0;
double a;
double b;
int index = 0; 
Servo myservo;
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

void setup() {
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
  myservo.write(90); 
  b = getDistance(Object_Trig, Object_Echo); 

  Serial.print("Object");
  Serial.println(b);
  delay(1000);
  b = getDistance(Object_Trig, Object_Echo);  
  Serial.print("Wall");
  Serial.println(b);
}