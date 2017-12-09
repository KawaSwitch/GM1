#pragma once
#include "Picking.h"

static GLdouble color_top[4] = { 0.9, 0.9, 1.0, 1.0 };
static GLdouble color_middle[4] = { 1.0, 1.0, 1.0, 1.0 };
static GLdouble color_bottom[4] = { 0.0, 0.0, 0.1, 1.0 };

// �w�i
struct Background
{
    void Draw() const
    {
        // �f�v�X�l�͖���
        glDisable(GL_DEPTH_TEST);

        glBegin(GL_QUADS);

        // ���삫����ƌv�Z������
        glColor3dv(color_top);
        glVertex3d(-1e6, 1e6, 0);
        glVertex3d(1e6, 1e6, 0);

        glColor3dv(color_middle);
        glVertex3d(1e6, -1e6, 0);
        glVertex3d(-1e6, -1e6, 0);
        
        //glColor3d(0.0, 0.8, 0.8);
        //glutSolidCube(1e4);
        glutPostRedisplay();

        glEnd();

        glEnable(GL_DEPTH_TEST);
    }
};