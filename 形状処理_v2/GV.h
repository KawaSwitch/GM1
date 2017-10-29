#pragma once

#include <gl/freeglut.h>
#include <math.h>
#include "define.h"

#ifdef GLOBAL_INSTANCE
#define GLOBAL
#else
#define GLOBAL extern 
#endif

#include "function.h"

GLOBAL int width, height; // ウィンドウサイズ

GLOBAL int mouse_X, mouse_Y; // マウス座標（回転用）
GLOBAL double xStart, yStart; // マウス座標（移動用）
GLOBAL double dist_X, dist_Y, dist_Z; // 移動距離

GLOBAL bool rotate_flag, move_flag; // 回転, 移動フラグ

GLOBAL double rot_mat[16]; // 回転マトリクス

// 構造体
GLOBAL Quaternion target;
GLOBAL Quaternion current;