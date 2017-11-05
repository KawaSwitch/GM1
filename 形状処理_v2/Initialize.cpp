#include "GV.h"
#include "Scene.h"
#include "Quaternion.h"
#include "Axis.h"
#include "Model.h"
#include "Triangle.h"
#include "Reader.h"

extern NormalAxis* axis;
extern Scene* scene;

void InitScene()
{
    scene = new Scene();

    // �t�@�C���ǂݍ���
    KjsReader* reader = new KjsReader("KJS_FILE/");
    auto objs = reader->GetObjectsFromKjsFolder();

    for (unsigned int i = 0; i < objs.size(); i++)
        scene->AddObject(objs[i]);

    // �����I�t�@�C���ǂݍ���
    scene->AddObject(reader->GetObjectFromFile("CGS_bspline_curve_1.kjs"));
    scene->AddObject(reader->GetObjectFromFile("CGS_bezier_surface_A.kjs"));
    scene->AddObject(reader->GetObjectFromFile("CGS_bspline_surface_1.kjs"));
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
    isUseLight = false; // �f�t�H���g�̓I�t

    //// �J�����O����
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    axis = new NormalAxis(0.4); // ��

    obj_number = 1; // �������ʎq��1

    rotate_flag = GL_FALSE;
    move_flag = GL_FALSE;

    InitQuaternion();
    InitScene();
}