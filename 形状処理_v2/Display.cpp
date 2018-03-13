#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"
#include "Background.h"
#include "Axis.h"
#include "ControlPoint.h"
#include "BsplineCurve.h"
#include "Test.h"
#include "Picking.h"

NormalAxis* axis; // ��
Scene* scene; // �V�[��
extern Scene* test_scene;

extern Point3d center;

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // ---- �r���[�s�� ----
    glLoadIdentity();
    gluLookAt(
        0.0, 0.0, 9.0,  // ���_�ʒu
        0.0, 0.0, 0.0,  // �����ʒu
        0.0, 1.0, 0.0   // ����� : y
    );
    // --------

    // ---- ���f���s�� ----

    glEnable(GL_STENCIL_TEST); // �X�e���V���L����

    // 1. �`��`��
    glPushMatrix();

    // �`��`��̌^���
    glStencilFunc(GL_ALWAYS, static_cast<int>(StencilRef::Entity), 0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

    glTranslated(dist_X, -dist_Y, dist_Z); // �ړ�
    glMultMatrixd(rot_mat); // ��]

    // �􉽃O���b�h�`��
    DrawGrid(GeoGrid2D());

    // �`��`��
    scene->Draw();
    // �e�X�g�`��
    TestDraw();

    glPopMatrix();


    // 2. �w�i�`��
    glPushMatrix();

    // �`�󂪕`�悳��Ă��Ȃ��ӏ��͔w�i��`��
    glStencilFunc(GL_EQUAL, static_cast<int>(StencilRef::Background), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    DrawBackground(BackgroundNormal());

    glPopMatrix();

    glDisable(GL_STENCIL_TEST); // �X�e���V��������


    // 3.���`��
    // TODO: �r���[�|�[�g��
    glPushMatrix();
    glScaled(1.3, 1.3, 1.3);
    glTranslated(-1.6, -1.6, -1.6);
    glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    // --------

    glutSwapBuffers();
}

// �R���\�[���ɐ�����\�����܂�
void ConsoleDiscription()
{
    puts("�`�󏈗�C++ ������@\n");

    puts("ESC : �I��\n");

    puts("$ �t�@�C���N�����ǂݍ���");
    puts("./KJS_FILE���Ŗ��O�̐擪��@������.kjs�t�@�C�����N�����ɓǂݍ��݂܂�\n");

    // �G���e�B�e�B����{�^������
    puts("$ �{�^���g�O���\���n");
    ShowButtonDiscription('B', "�~�j�}�N�X�{�b�N�X");
    ShowButtonDiscription('P', "����_");
    ShowButtonDiscription('F', "��K����(�ڐ�)�x�N�g��");
    ShowButtonDiscription('S', "��K�����x�N�g��");
    ShowButtonDiscription('C', "�ȗ��x�N�g��");
    ShowButtonDiscription('N', "�@���x�N�g��");
}

// �{�^���̐�����\�����܂�
void ShowButtonDiscription(char button, string disc)
{
    printf("    ");
    printf("%c : ", button);
    printf("%s\n", disc.c_str());
}