

#include "ArmController.h"
#include "SainSmart6Arm.h"

void ArmController::attach(int pin[SERVO_NUM])
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
    float r1, r2, r3;
    float r10 = s[0].read();
    float r20 = s[1].read();
    float r30 = s[2].read();

    for (int step = 0; step < STEP_COUNT; ++step)
    {
        current += unit;

        // TODO APIがアームに依存している
        if (!move(current.x, current.y, current.z, r1, r2, r3))
        {
            return false;
        }

        uint8_t s1speed = calcSpeed(r10, r1, sPerStep);
        uint8_t s2speed = calcSpeed(r20, r2, sPerStep);
        uint8_t s3speed = calcSpeed(r30, r3, sPerStep);

        seqes[0][step] = {s[0].toMicroseconds(r1), s1speed};
        seqes[1][step] = {s[1].toMicroseconds(r2), s2speed};
        seqes[2][step] = {s[2].toMicroseconds(r3), s3speed};

        r10 = r1;
        r20 = r2;
        r30 = r3;
    }
}
