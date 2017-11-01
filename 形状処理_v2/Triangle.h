#pragma once

#include "GV.h"
#include "Object.h"

// �f�o�O�N���X
// �O�p�`��
class Triangle : public Object
{
private:

    Point3d A, B, C;

    // ���O�`��
    virtual void PreDraw() override
    {
        glColor3d(1.0, 0.0, 0.0); // �Ƃ肠������
        glBegin(GL_TRIANGLES);
        glVertex3d(A.X, A.Y, A.Z);
        glVertex3d(B.X, B.Y, B.Z);
        glVertex3d(C.X, C.Y, C.Z);
        glEnd();
    }

public:

    Triangle(Point3d a, Point3d b, Point3d c) { A = a; B = b; C = c; }
    ~Triangle() { }
};