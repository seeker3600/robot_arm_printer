#include <VarSpeedServo.h>
#include <FABRIK2D.h>

#define MOVE_UNIT (2.0f)
#define SERVO_SPEED (10)

VarSpeedServo s[6];

//int lengths[] = {120, 125}; //形状的にはこちらが近い
int lengths[] = {150, 105}; //構造による補正値
Fabrik2D fabrik2D(3, lengths);

/*
s2とs3の角度を制限する。
r2: s2の角度。
r3: s3の角度。
r3current: s3の現在の角度。
           省略すると、s3の角度は制限せず、s2のみを制限する。
*/
void constrainS2S3(float& r2, float& r3, float r3current = -1)
{
  if(r3current >= 0 && r3 > r3current)
  {
    r3 = constrain(r3, 30, 190 - r2);
  }

  r2 = constrain(r2, 30, 190 - r3);

  r2 = constrain(r2, 30, 160);
  r3 = constrain(r3, 30, 160);
}

/*
FABRIK2Dのモデルをロボットアームのサーボモータの角度に変換する。
*/
void convert(float& r2, float& r3)
{
  r2 = -r2 - r3;
}

void setup() {

  s[0].attach(2);
  s[1].attach(3);
  s[2].attach(4);
  s[3].attach(5);
  s[4].attach(6, 20, 160);
  s[5].attach(7);

  fabrik2D.setTolerance(0.5);

  //pinMode(8, INPUT_PULLUP);
  Serial.begin(9600);
}

bool wrote = false;

void move(float x, float y)
{
  fabrik2D.solve(x, y, lengths);

  float r3 = fabrik2D.getAngle(0) * 57296.0f / 1000.0f;
  float r2 = fabrik2D.getAngle(1) * 57296.0f / 1000.0f;
  convert(r2, r3);

  Serial.print("X = ");
  Serial.print(x);
  Serial.print(", Y = ");
  Serial.print(y);
  Serial.print(", R2 = ");
  Serial.print(r2);
  Serial.print(", R3 = ");
  Serial.println(r3);

  constrainS2S3(r2, r3, s[2].read());

  VarSpeedServo* s2 = &s[1];
  VarSpeedServo* s3 = &s[2];

  if(wrote) s2->wait();
  if(wrote) s3->wait();
  s2->write(s2->toMicroseconds(r2), SERVO_SPEED, false);
  s3->write(s3->toMicroseconds(r3), SERVO_SPEED, false);
  wrote = true;
}

float y = 0;
int mode = 0;

void position()
{
  switch(mode)
  {
    case 0:
      y -= MOVE_UNIT;
      if(y == -10.0f) ++mode;
      break;
    case 1:
      y += MOVE_UNIT;
      if(y == 80.0f) mode = 0;
      break;
  }
}

void loop() {
  //return;
  position();
  move(80.0f, y);
}
