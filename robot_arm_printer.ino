
#include "ArmController.h"

ArmController arm;

void setup()
{
  Serial.begin(9600);

  pinMode(8, INPUT_PULLUP);

  int pins[] = {2, 3, 4, 5, 6, 7};
  arm.attach(pins);
}

void loop()
{
  //return;
  point3d near = {100.0, 0.0f, -50.0f};
  point3d far = {100.0, 0.0f, 50.0f};

  //TODO 非同期処理を追加
  arm.solve(near, far, 1.0f);
  arm.play();
  while (digitalRead(8) == HIGH){}

  arm.solve(far, near, 1.0f);
  arm.play();
  while (digitalRead(8) == HIGH){}
}