

#include "ArmController.h"
#include "SainSmart6Arm.h"

ArmController::ArmController(int pin[SERVO_NUM])
{
    for (int n = 0; n < SERVO_NUM; ++n)
    {
        s[n].attach(pin[n]);
    }
}

void ArmController::play()
{
    for (int step = 0; step < STEP_COUNT; ++step)
    {
        for (int n = 0; n < SERVO_NUM; ++n)
        {
            auto seq = seqes[n];
            s[n].write(seq[step].microsec, seq[step].speed, false);
        }

        bool moving;
        do
        {
            delay(1);
            moving = false;

            for (int n = 0; n < SERVO_NUM && !moving; ++n)
            {
                moving = s[n].isMoving();
            }

        } while (moving);
    }
}

bool ArmController::solve(point3d before, point3d after, float second)
{
    const point3d unit = (after - before) / (float)STEP_COUNT;
    const float sPerStep = second / (float)STEP_COUNT;

    point3d current = before;
    float r1, r2, r3, r10, r20, r30;

    for (int n = 0; n < STEP_COUNT; ++n)
    {
        current += unit;

        // TODO APIがアームに依存している
        if (!move(current.x, current.y, current.z, r1, r2, r3))
        {
            return false;
        }

        uint8_t s1speed = n > 0 ? calcSpeed(r10, r1, sPerStep) : 10;
        uint8_t s2speed = n > 0 ? calcSpeed(r20, r2, sPerStep) : 10;
        uint8_t s3speed = n > 0 ? calcSpeed(r30, r3, sPerStep) : 10;

        seqes[0][n] = {s[0].toMicroseconds(r1), s1speed};
        seqes[1][n] = {s[1].toMicroseconds(r2), s2speed};
        seqes[2][n] = {s[2].toMicroseconds(r3), s3speed};
        seqes[3][n] = {0, 0};
        seqes[4][n] = {0, 0};
        seqes[5][n] = {0, 0};

        //Serial.print(fabrik2D.getX(0));
        //Serial.print("\t");
        //Serial.print(fabrik2D.getY(0));
        //Serial.println();

        r10 = r1;
        r20 = r2;
        r30 = r3;
    }
}
