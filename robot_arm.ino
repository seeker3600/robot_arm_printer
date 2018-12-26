#include <VarSpeedServo.h>

#define MOVE_UNIT (5)
#define MOVE_SPEED (50)

VarSpeedServo s[6];
int curIdx = 0;

//int constrainS5(int value)
//{
//  return constrain(value, 20, 160);
//}

void constrainS2S3(int& r2, int& r3)
{
  bool up2 = false;

  if(r2 < 0)
    r2 = s[1].read();
  else
    up2 = (r2 - s[1].read()) >= 0;

  bool up3 = false;
  
  if(r3 < 0)
    r3 = s[2].read();
  else
    up3 = (r3 - s[2].read()) >= 0;

  if(up2)
    r2 = constrain(r2, 30, 190 - r3);
  else if(up3)
    r2 = constrain(r2, 30, 190 - r3);
    //r3 = constrain(r3, 30, 190 - r2);
    
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
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  Serial.begin(9600);
}

void chmod()
{
  if (digitalRead(8) == LOW)
    --curIdx;  
  else if (digitalRead(9) == LOW)
    ++curIdx;  
    
  curIdx = constrain(curIdx, 0, 5);
}

void loop() {
  delay(100);

  chmod();

  int rot = 0;
  if (digitalRead(10) == LOW)
    rot -= MOVE_UNIT;  
  else if (digitalRead(11) == LOW)
    rot += MOVE_UNIT;
  else
    return;

  rot += s[curIdx].read();

  if(curIdx == 1)
  {
    int rot3 = -1;
    constrainS2S3(rot, rot3);
    if(rot3 >= 0)
      s[2].write(rot3, MOVE_SPEED, true);
  }
  else if(curIdx == 2)
  {
    int rot2 = -1;
    constrainS2S3(rot2, rot);
    if(rot2 >= 0)
      s[1].write(rot2, MOVE_SPEED, true);
  }

  s[curIdx].write(rot, MOVE_SPEED, true);

  Serial.print(curIdx);
  Serial.print(", ");
  Serial.println(rot);
}
