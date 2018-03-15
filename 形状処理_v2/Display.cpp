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

    // ���N�G�X�g������Ε\���������ʒu�ɖ߂�
    if (isViewInitRequested)
    {
        InitQuaternion(); // ��]�p����������
        dist_X = dist_Y = dist_Z = 0.0; // �ړ���������

        glutPostRedisplay();
        isViewInitRequested = false;
    }

    glEnable(GL_STENCIL_TEST); // �X�e���V���L����

    // 1.���`��
    if (isShowAxis)
    {
        // ���̌^���
        glStencilFunc(GL_ALWAYS, static_cast<int>(StencilRef::Axis), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        // ���p�r���[�|�[�g�T�C�Y���v�Z
        int axisViewWidth = width / 5;
        int axisViewHeight = height / 5;
        int axisViewSize = (axisViewWidth < axisViewHeight) ? axisViewWidth : axisViewHeight;

        // ���p�ɕϊ��s����w�肵�Ȃ���
        glPushMatrix();

        glViewport(axisViewSize / 10, axisViewSize / 10, axisViewSize, axisViewSize);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(30.0, 1.0, 0.1, 10.0); // ���̓A�X��Œ�(1.0)

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            0.0, 0.0, 2.0,  // ���_�ʒu
            0.0, 0.0, 0.0,  // �����ʒu
            0.0, 1.0, 0.0   // ����� : y
        );

        glMultMatrixd(rot_mat); // ��]�ʂ̓��f���Ɠ�����
        axis->Draw();

        glPopMatrix();

        // ���ɖ߂�
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity(); // TODO: ���̕�near�Ƃ�far�Ƃ����ϐ��u���Ĉꊇ�Ǘ�������
        gluPerspective(30.0, (GLdouble)width / height, 0.1, 10000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            0.0, 0.0, 9.0,  // ���_�ʒu
            0.0, 0.0, 0.0,  // �����ʒu
            0.0, 1.0, 0.0   // ����� : y
        );
    }

    // 2. �`��`��
    glPushMatrix();

    // �`��̌^���
    // ���������ĕ`��
    glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Entity), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glTranslated(dist_X, -dist_Y, dist_Z); // �ړ�

    glMultMatrixd(rot_mat); // ��]

    // �`��`��
    scene->Draw();
    // �e�X�g�`��
    TestDraw();


    // 3. �O���b�h�`��
    
    // �O���b�h�̌^���
    // ��̑S�����`�悳��Ă��Ȃ����ɃO���b�h��`��
    glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Grid), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // �􉽃O���b�h�`��
    DrawGrid(GeoGrid2D());

    glPopMatrix();


    // 4. �w�i�`��
    glPushMatrix();

    // ��̑S�����`�悳��Ă��Ȃ��ӏ��͔w�i��`��
    glStencilFunc(GL_EQUAL, static_cast<int>(StencilRef::Background), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    DrawBackground(BackgroundNormal());

    glPopMatrix();

    glDisable(GL_STENCIL_TEST); // �X�e���V��������

    // --------

    glutSwapBuffers();
}

// �R���\�[���ɐ�����\�����܂�
void ConsoleDiscription()
{
    puts("�`�󏈗�C++ ������@\n");

    puts("$ �t�@�C���N�����ǂݍ���");
    puts("./KJS_FILE���Ŗ��O�̐擪��@������.kjs�t�@�C�����N�����ɓǂݍ��݂܂�\n");

    puts("$ �}�E�X����");
    ShowButtonDiscription("�E�h���b�O", "��]");
    ShowButtonDiscription("���h���b�O", "�ړ�");
    ShowButtonDiscription("�z�C�[��", "�g��/�k��");

    puts("");

    // �G���e�B�e�B����{�^������
    puts("$ �G���e�B�e�B�g�O���\���n");
    ShowButtonDiscription("B", "�~�j�}�N�X�{�b�N�X");
    ShowButtonDiscription("P", "����_");
    ShowButtonDiscription("F", "��K����(�ڐ�)�x�N�g��");
    ShowButtonDiscription("S", "��K�����x�N�g��");
    ShowButtonDiscription("C", "�ȗ��x�N�g��");
    ShowButtonDiscription("N", "�@���x�N�g��");

    puts("");

    puts("$ ���̑��\���n");
    ShowButtonDiscription("ESC", "�I��");
    ShowButtonDiscription("A", "���\���g�O��");
    ShowButtonDiscription("I", "��]/�ړ������ɖ߂�");
    //ShowButtonDiscription("L", "���C�e�B���O�g�O��");

    puts("\n");

    puts("�f�o�O�p�o�́�\n");
}

// �{�^���̐�����\�����܂�
void ShowButtonDiscription(char* button, char* disc)
{
    printf("    ");
    printf("%s : ", button);
    printf("%s\n", disc);
}