#include <VarSpeedServo.h>
#include <FABRIK2D.h>

#define MOVE_UNIT (5)
#define MOVE_SPEED (50)

VarSpeedServo s[6];

int lengths[] = {225, 150};
Fabrik2D fabrik2D(3, lengths);

/*
s2とs3の角度を制限する。
r2: s2の角度。
r3: s3の角度。
r3current: s3の現在の角度。
           省略すると、s3の角度は制限せず、s2のみを制限する。
*/
void constrainS2S3(int& r2, int& r3, int r3current = -1)
{
  if(r3current >= 0 && r3 > r3current)
  {
    r3 = constrain(r3, 30, 190 - r2);
  }

  r2 = constrain(r2, 30, 190 - r3);

  r2 = constrain(r2, 30, 160);
  r3 = constrain(r3, 30, 160);
}

void setup() {

  s[0].attach(2);
  s[1].attach(3);
  s[2].attach(4);
  s[3].attach(5);
  s[4].attach(6, 20, 160);
  s[5].attach(7);

  pinMode(8, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
}
