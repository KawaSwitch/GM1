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
GeoGrid2D* grid; // �O���b�h
Scene* scene; // �V�[��
extern Scene* test_scene;

Point3d rotateCenter; // ��]���S

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

        // ���p�r���[�|�[�g��ݒ�
        double axisLength = axis->GetLength();
        glViewport(axisViewSize / 10, axisViewSize / 10, axisViewSize, axisViewSize);

        // ���p�ɕϊ��s����w�肷��
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(
            -axisLength, axisLength,
            -axisLength, axisLength,
            -axisLength, axisLength);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // ���p���C�g�̂݃I���ɂ���
        for (const auto& light : modelLight)
            light->Off();
        for (const auto& light : axisLight)
            light->On();

        glPushMatrix();

        glMultMatrixd(rot_mat); // ��]�ʂ̓��f���Ɠ�����
        axis->Draw();

        glPopMatrix();
    }


    // �r���[�|�[�g���E�B���h�E�S�̂ɐݒ�
    glViewport(0, 0, width, height);

    // �ϊ��s���S�̗p�Ɏw�肵�Ȃ���
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // TODO: ���̕�near�Ƃ�far�Ƃ����ϐ��u���Ĉꊇ�Ǘ�������
    gluPerspective(30.0, (GLdouble)width / height, 0.1, 10000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0, 0.0, 100.0,  // ���_�ʒu
        0.0, 0.0, 0.0,  // �����ʒu
        0.0, 1.0, 0.0   // ����� : y
    );

    // �`��p���C�g�̂݃I���ɂ���
    for (const auto& light : axisLight)
        light->Off();
    for (const auto& light : modelLight)
        light->On();


    // 2. �`��`��
    {
        // �`��̌^���
        // ���������ĕ`��
        glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Entity), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glPushMatrix();
        {
            // �N���ŏ��̕`��ŉ�]���S���E�B���h�E���S�ɂ���
            SetRotateCenter();
            glTranslated(-rotateCenter.X, -rotateCenter.Y, -rotateCenter.Z);

            glTranslated(dist_X, -dist_Y, dist_Z); // �ړ�

            // ��]���S���w�肵�ĉ�]
            // ���f�������_�ɖ߂��ĉ�]����(�s��̊|���Z�͋t!)
            glTranslated(rotateCenter.X, rotateCenter.Y, rotateCenter.Z);
            glMultMatrixd(rot_mat); // ��]
            glTranslated(-rotateCenter.X, -rotateCenter.Y, -rotateCenter.Z);

            // �`��`��
            scene->Draw();
            // �e�X�g�`��
            TestDraw();

            // ��]���S�`��
            ShowRotateCenter(rotate_flag);

            // �O���b�h�`��
            {
                // �O���b�h�̌^���
                // ��̑S�����`�悳��Ă��Ȃ����ɃO���b�h��`��
                glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::Grid), 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

                // �􉽃O���b�h�`��
                if (gridType != GridType::InVisible)
                    grid->Draw();
            }
        }
        glPopMatrix();
    }


    // 3. �w�i�`��
    {
        // ��̑S�����`�悳��Ă��Ȃ��ӏ��͔w�i��`��
        glStencilFunc(GL_EQUAL, static_cast<int>(StencilRef::Background), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        DrawBackground(BackgroundNormal());
    }


    glDisable(GL_STENCIL_TEST); // �X�e���V��������

    //isFirst = false;
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

        glStencilFunc(GL_GEQUAL, static_cast<int>(StencilRef::RotateCenter), 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        // �`��
        glColor4dv(Color::orange);
        glPointSize(10.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Color::orange);

        // �f�v�X�l�͕]�����Ȃ�
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
    ShowButtonDiscription("G", "�O���b�h�\���ύX");
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