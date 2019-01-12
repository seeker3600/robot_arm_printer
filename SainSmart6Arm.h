#ifndef SAIN_SMART_6_ARM_H
#define SAIN_SMART_6_ARM_H

#include <Arduino.h>

/*
指定した位置にロボットアームの先端を移動するためのモータ角度を計算する。
現時点ではs3制限は無効。
*/
bool move(float x, float y, float z, float &r1, float& r2, float& r3);
uint8_t calcSpeed(float r0, float r, float sec);

#endif
