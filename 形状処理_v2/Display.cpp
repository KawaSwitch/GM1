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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();

    // �ړ�
    glTranslated(dist_X, -dist_Y, dist_Z);
    // ��]
    glMultMatrixd(rot_mat);

    // �􉽃O���b�h�`��
    DrawGrid(GeoGrid2D());
    // �`��`��
    scene->Draw();
    // �e�X�g�`��
    TestDraw();

    glPopMatrix();

    // ���`��
    glPushMatrix();
    glScaled(1.3, 1.3, 1.3);
    glTranslated(-1.6, -1.6, -1.6);
    glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    glutSwapBuffers();
}