#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"
#include "Model.h"
#include "Triangle.h"
#include "Reader.h"

extern NormalAxis* axis;
extern Scene* scene;
extern Scene* test_scene;

void InitScene()
{
    scene = new Scene();
    test_scene = new Scene(); // �e�X�g�p�������ŏ�����

    // @�t�@�C���ǂݍ���
    auto reader = new KjsReader("KJS_FILE/");
    auto objs = reader->GetObjectsFromKjsFolder();

    for (unsigned int i = 0; i < objs.size(); i++)
        scene->AddObject(objs[i]);
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
    glewInit(); // glew�g��

    glClearColor(1.0, 1.0, 1.0, 1.0);   // �w�i�F�F��

    glEnable(GL_DEPTH_TEST); // �f�v�X�l �L����

    glEnable(GL_BLEND); // �u�����h �L����
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mainLight.InitLight(); // ���C�����C�g������
    isUseLight = true; // �f�t�H���g�̓I�t �� �Ƃ肠�����I��(�K�v�ȂƂ���̂ݏ������{��)

    //// �J�����O����
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    axis = new NormalAxis(0.4); // ��

    obj_number = 1; // �������ʎq��1

    // ��]�ړ��n������
    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;
    InitQuaternion();

    // �V�[��������
    InitScene();
}