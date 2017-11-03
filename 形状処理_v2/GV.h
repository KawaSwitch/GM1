#pragma once

using namespace std;

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <math.h>
#include <functional>
#include "define.h"
#include "EPS.h"
#include "Error.h"
#include "Color.h"
#include "Vector.h"
#include "Point3d.h"
#include "Matrix.h"

#ifdef GLOBAL_INSTANCE
#define GLOBAL
#else
#define GLOBAL extern 
#endif

#include "function.h"

GLOBAL int width, height; // �E�B���h�E�T�C�Y

GLOBAL int mouse_X, mouse_Y; // �}�E�X���W�i��]�p�j
GLOBAL double xStart, yStart; // �}�E�X���W�i�ړ��p�j
GLOBAL double dist_X, dist_Y, dist_Z; // �ړ�����

GLOBAL bool rotate_flag, move_flag; // ��], �ړ��t���O

GLOBAL double rot_mat[16]; // ��]�}�g���N�X

GLOBAL unsigned int obj_number; // �I�u�W�F�N�g���ʎq

// �\����
GLOBAL Quaternion target;
GLOBAL Quaternion current;

// �`��A�V�X�g
template<class Anything> void DrawWithoutSetting(const Anything& obj)
{
    // �f�o�O�`��p
    obj.Draw();
}
