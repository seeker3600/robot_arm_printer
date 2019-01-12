#include "SainSmart6Arm.h"

#include <FABRIK2D.h>

#define radToDeg(__rad) ((__rad) * 57296.0f / 1000.0f)

int lengths[] = {120, 127/*肘短辺20mm、底辺125mmの直線距離(126.59mm)*/};
Fabrik2D fabrik2D(3, lengths);

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
void convert(T& r1, T& r2, T& r3)
{
  r2 = -r2 - r3
    + 30/*s2の取付角度補正*/
    +  9/*肘の理不尽構造による直角三角形の角度(9.09度)*/
    +  3/*実測値*/
    ;

  r3 = r3
    +  2/*実測値*/
    ;

  r1 = r1
    + 90/*s1の取付角度補正*/
    ;
}

/*
指定した位置にロボットアームの先端を移動するためのモータ角度を計算する。
*/
bool move(float x, float y, float z, float &r1, float& r2, float& r3)
{
  fabrik2D.setTolerance(0.5);

  if(!fabrik2D.solve2(x, y, z, lengths)) return false;

  r1 = radToDeg(fabrik2D.getBaseAngle());
  r3 = radToDeg(fabrik2D.getAngle(0));
  r2 = radToDeg(fabrik2D.getAngle(1));
  convert(r1, r2, r3);
  constrainS2S3(r2, r3);

  //Serial.print(fabrik2D.getX(2));
  //Serial.print("\t");
  //Serial.print(fabrik2D.getY(2));
  //Serial.println();

  return true;
}

/*
回転速度を計算する。ステップが一定時間で完了するようにする。
*/
uint8_t calcSpeed(float r0, float r, float sec)
{
  float sa = r0 - r;
  int speed = abs(sa) / (2.2809 * sec) - 0.16765;
  return constrain(speed, 1, 255);
}
