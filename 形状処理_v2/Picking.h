#pragma once

#include "GV.h"

// ���[�J�����W�̃f�v�X�l���擾����
float GetDepth(int x, int y);

// �f�v�X�l���烏�[���h���W���擾����
Point3d GetWorldCoord(int x, int y, float depth);

unsigned int GetObjNumberOnMousePointer(int x, int y);

unsigned int GetNearestNumber(GLuint hits, GLuint *buf);
unsigned int GetFarthestNumber(GLuint hits, GLuint* buf);