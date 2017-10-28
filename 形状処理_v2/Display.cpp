#include "GV.h"
#include "Callback.h"
#include "GeoGrid.h"

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // �ړ�
    glTranslated(dist_X, -dist_Y, dist_Z);
    // ��]
    glMultMatrixd(rotate);

    glPopMatrix();

    // �􉽃O���b�h�`��
    glPushMatrix();
    DrawGrid(GeoGrid2D());
    glPopMatrix();

    glutSwapBuffers();
}