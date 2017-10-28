#include "GV.h"
#include "Quaternion.h"

void Initialize()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);   // �w�i�F�F��

    glEnable(GL_DEPTH_TEST); // �f�v�X�l �L����

    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;

    // ��]�s��
    for (int i = 0; i < 16; i++)
        rotate[i] = 0.0;

    // �N�H�[�^�j�I��
    target.x = 0.0; target.y = 0.0;
    target.z = 0.0; target.w = 0.0;
    current.x = 1.0; current.y = 0.0;
    current.z = 0.0; current.w = 0.0;

    // �N�H�[�^�j�I��������
    CalcRotateMatrix(rotate, current);
}