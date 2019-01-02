#include <VarSpeedServo.h>
#include <FABRIK2D.h>

/*
s2とs3の角度を制限する。
r2: s2の角度。
r3: s3の角度。
r3current: s3の現在の角度。
           省略すると、s3の角度は制限せず、s2のみを制限する。
*/
template<typename T>
void constrainS2S3(T& r2, T& r3, T r3current = -1)
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
template<typename T>
void convert(T& r2, T& r3)
{
  r2 = -r2 - r3;
}

//int lengths[] = {120, 125}; //形状的にはこちらが近い
int lengths[] = {150, 105}; //構造による補正値
Fabrik2D fabrik2D(3, lengths);

void move(float x, float y, float& r2, float& r3)
{
  fabrik2D.solve(x, y, lengths);

  r3 = fabrik2D.getAngle(0) * 57296.0f / 1000.0f;
  r2 = fabrik2D.getAngle(1) * 57296.0f / 1000.0f;
  convert(r2, r3);
  constrainS2S3(r2, r3);
}

#define Y_MIN (-10.0f)
#define Y_MAX ( 70.0f)
#define STEP (80)
#define S_PER_STEP (0.03125f)

servoSequencePoint s2seq[STEP];
servoSequencePoint s3seq[STEP];

servoSequencePoint resetseq[2] = {{90, 10}, {91, 10}};

uint8_t calcSpeed(float r0, float r)
{
  float sa = r0 - r;
  int speed = abs(sa) / (2.2809 * S_PER_STEP) - 0.16765;
  return constrain(speed, 1, 255);
}

void initSeq()
{
  const float unit = (Y_MAX - Y_MIN) / (float)STEP;
  float y = Y_MIN;
  float r2, r3, r20, r30;

  for(int n = 0; n < STEP; ++n)
  {
    move(80.0f, y, r2, r3);

    uint8_t s2speed = n > 0 ? calcSpeed(r20, r2) : 10;
    uint8_t s3speed = n > 0 ? calcSpeed(r30, r3) : 10;

    s2seq[n] = {(uint8_t)r2, s2speed};
    s3seq[n] = {(uint8_t)r3, s3speed};

    Serial.print(n);
    Serial.print(": r2 = \t");
    Serial.print(s2seq[n].position);
    Serial.print(", r3 = \t");
    Serial.println(s3seq[n].position);

    y += unit;
    r20 = r2;
    r30 = r3;
  }
}

VarSpeedServo s[6];

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

void reset()
{
  s[1].write(s2seq[0].position, 20);
  s[2].write(s3seq[0].position, 20);

  s[1].wait();
  s[2].wait();

  delay(1000);
}

void play()
{
  for(int n = 1; n < STEP; ++n)
  {
    s[1].write(s2seq[n].position, s2seq[n].speed, false);
    s[2].write(s3seq[n].position, s3seq[n].speed, false);

    while(s[1].isMoving() || s[2].isMoving()) { delay(1); }
  }

  delay(1000);
}

void loop() {
  //return;

  reset();
  play();

  while(digitalRead(8) == HIGH);
}