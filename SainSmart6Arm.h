#ifndef SAIN_SMART_6_ARM_H
#define SAIN_SMART_6_ARM_H

#include <FABRIK2D.h>

// いずれは隠したい
extern Fabrik2D fabrik2D;

/*
指定した位置にロボットアームの先端を移動するためのモータ角度を計算する。
現時点ではs3制限は無効。
*/
bool move(float x, float y, float z, float &r1, float& r2, float& r3);

#endif
