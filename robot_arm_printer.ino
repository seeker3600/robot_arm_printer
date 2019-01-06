
#include "SainSmart6Arm.h"
#include <VarSpeedServo.h>

#define Y_MIN (-30.0f)
#define Y_MAX (130.0f)
#define STEP (10)
#define S_PER_STEP (0.25)

VarSpeedServo s[6];

/*
回転速度を計算する。ステップが一定時間で完了するようにする。
*/
uint8_t calcSpeed(float r0, float r, float sec)
{
  float sa = r0 - r;
  int speed = abs(sa) / (2.2809 * sec) - 0.16765;
  return constrain(speed, 1, 255);
}

typedef struct 
{
  int microsec;
  uint8_t speed;
} servoParam;

servoParam s2seq[STEP];
servoParam s3seq[STEP];

void initSeq()
{
  const float unit = (Y_MAX - Y_MIN) / (float)STEP;
  float y = Y_MIN;
  float r2, r3, r20, r30;

  for(int n = 0; n < STEP; ++n)
  {
    move(120.0f, y, r2, r3);

    uint8_t s2speed = n > 0 ? calcSpeed(r20, r2, S_PER_STEP) : 10;
    uint8_t s3speed = n > 0 ? calcSpeed(r30, r3, S_PER_STEP) : 10;

    s2seq[n] = {s[1].toMicroseconds(r2), s2speed};
    s3seq[n] = {s[2].toMicroseconds(r3), s3speed};

    Serial.print(fabrik2D.getX(0));
    Serial.print("\t");
    Serial.print(fabrik2D.getY(0));
    Serial.print("\t");
    Serial.print(fabrik2D.getX(1));
    Serial.print("\t");
    Serial.print(fabrik2D.getY(1));
    Serial.print("\t");
    Serial.print(fabrik2D.getX(2));
    Serial.print("\t");
    Serial.print(fabrik2D.getY(2));
    Serial.println();

    y += unit;
    r20 = r2;
    r30 = r3;
  }
}

void reset()
{
  s[1].write(s2seq[0].microsec, 20);
  s[2].write(s3seq[0].microsec, 20);

  s[1].wait();
  s[2].wait();

  delay(1000);
}

void play()
{
  for(int n = 1; n < STEP; ++n)
  {
    s[1].write(s2seq[n].microsec, s2seq[n].speed, false);
    s[2].write(s3seq[n].microsec, s3seq[n].speed, false);

    while(s[1].isMoving() || s[2].isMoving()) { delay(1); }
  }

  delay(1000);
}

void setup() {
  Serial.begin(9600);

  s[0].attach(2);
  s[1].attach(3);
  s[2].attach(4);
  s[3].attach(5);
  s[4].attach(6, 20, 160);
  s[5].attach(7);

  pinMode(8, INPUT_PULLUP);

  fabrik2D.setTolerance(0.5);
  initSeq();
}

void loop() {
  //return;

  reset();
  while(digitalRead(8) == HIGH);

  play();
  while(digitalRead(8) == HIGH);
}