#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"

extern NormalAxis* axis;
extern Scene* scene;

void InitScene()
{
    scene = new Scene();

    // �e�I�u�W�F�N�g�ǉ�

}

void InitQuaternion()
{
    // ��]�s��
    for (int i = 0; i < 16; i++)
        rot_mat[i] = 0.0;

    // �N�H�[�^�j�I��
    target.x = 0.0; target.y = 0.0;
    target.z = 0.0; target.w = 0.0;
    current.x = 0.0; current.y = 0.0;
    current.z = 0.0; current.w = 1.0;

    // �N�H�[�^�j�I��������
    CalcRotateMatrix(rot_mat, current);
}

void Initialize()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);   // �w�i�F�F��

    glEnable(GL_DEPTH_TEST); // �f�v�X�l �L����

    axis = new NormalAxis(0.4); // ��

    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;

    InitQuaternion();
    InitScene();
}