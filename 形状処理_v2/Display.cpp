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

Point3d rotateCenter; // ��]���S

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

    // ��]���S���w�肵�ĉ�]
    // ���f�������_�ɖ߂��ĉ�]����(�s��̊|���Z�͋t!)
    glTranslated(rotateCenter.X, rotateCenter.Y, 0);
    glMultMatrixd(rot_mat); // ��]
    glTranslated(-rotateCenter.X, -rotateCenter.Y, 0);

    // �`��`��
    scene->Draw();
    // �e�X�g�`��
    TestDraw();

    // ��]���S�`��
    ShowRotateCenter(rotate_flag);


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

// ��]���S��ݒ肵�܂�
void SetRotateCenter()
{
    vector<Box> sceneBoxes;

    // ��]���S�����߂�
    // ���ׂĂ̌`��̒��S����]���S�Ƃ���
    // TODO: ������ʒ��S�ŉ�]��������
    sceneBoxes.push_back(scene->GetCoverBound());
    sceneBoxes.push_back(test_scene->GetCoverBound());

    auto center = Box(sceneBoxes).Center();
    rotateCenter = Point3d(center.X, center.Y, center.Z);
}
// ��]���S��\�����܂�
void ShowRotateCenter(bool isRotating)
{
    if (rotate_flag)
    {
        SetRotateCenter();

        // ���C�e�B���O�͐؂��Ă���
        if (glIsEnabled(GL_LIGHTING))
            glDisable(GL_LIGHTING);

        // �`��̌^���
        // ���������ĕ`��
        glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::RotateCenter), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        // �`��
        glColor3dv(Color::orange);
        glPointSize(10.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::orange);

        glDisable(GL_DEPTH_TEST);
        glBegin(GL_POINTS);
        glVertex3d(rotateCenter);
        glEnd();
        glEnable(GL_DEPTH_TEST);

        glPointSize(1.0);
    }
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