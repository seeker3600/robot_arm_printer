#ifndef ARM_CONTROLLER_H
#define ARM_CONTROLLER_H

#include <VarSpeedServo.h>

#define SERVO_NUM (3)
#define STEP_COUNT (10)

struct point3d
{
  float x;
  float y;
  float z;

  point3d operator-(point3d b)
  {
    return {x - b.x, y - b.y, z - b.z};
  }

  void operator+=(point3d b)
  {
    x += b.x;
    y += b.y;
    z += b.z;
  }

  point3d operator/(float b)
  {
    return {x / b, y / b, z / b};
  }

};

struct servoParam
{
  int microsec;
  uint8_t speed;
};

class ArmController
{
private:
  VarSpeedServo s[SERVO_NUM];
  servoParam seqes[SERVO_NUM][STEP_COUNT];

public:
  void attach(int pin[SERVO_NUM]);
  
  void play();

  bool solve(
      point3d before,
      point3d after,
      float second);
};

#endif
