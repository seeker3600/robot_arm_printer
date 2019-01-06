#ifndef SAIN_SMART_6_ARM_H
#define SAIN_SMART_6_ARM_H

#include <FABRIK2D.h>

// いずれは隠したい
extern Fabrik2D fabrik2D;

template<typename T>
void constrainS2S3(T& r2, T& r3, T r3current = -1);

template<typename T>
void convert(T& r2, T& r3);

bool move(float x, float y, float& r2, float& r3);

#endif
