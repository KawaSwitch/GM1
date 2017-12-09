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
    gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // ���`��
    // ������ł̓��f���r���[�s���ς��Ȃ�
    glPushMatrix();
    glTranslated(-2.0, -2.0, -2.0);
    glMultMatrixd(rot_mat);
    axis->Draw();
    glPopMatrix();

    // �ړ�
    glTranslated(dist_X, -dist_Y, dist_Z);

    // ��]
    //glTranslated(center.X, center.Y, center.Z);
    glMultMatrixd(rot_mat);
    //glTranslated(-center.X, -center.Y, -center.Z);
    
    //scene_mat = Matrix3d::CreateIdentity();
    //glMultMatrixd(scene_mat);
    //glLoadMatrixd(scene_mat);

    //printf("��]���S:%f %f %f\n", center.X, center.Y, center.Z);

    //// ��]���S�`��
    //glColor3dv(Color::red);
    //glPointSize(10.0);
    //glBegin(GL_POINTS);
    //glVertex3d(center.X, center.Y, center.Z);
    //glEnd();

    // �`��`��
    scene->Draw();

    // �e�X�g�`��
    TestDraw();

    // �􉽃O���b�h�`��
    //DrawGrid(GeoGrid2D());

    glutSwapBuffers();
}