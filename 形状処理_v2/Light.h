#pragma once

#include "GV.h"

// �����N���X
// ���C���ɂ����g���\��Ȃ��̂œK��
class Light
{
private:

    // �K��
    GLfloat lightPosition[4] = { 0, 0, 100, 1.0 }; // �����̈ʒu
    GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 }; // �g�U��
    GLfloat lightAmbient[4] = { 0.0, 0.0, 0.0, 1.0 }; // ����
    GLfloat lightSpecular[4] = { 1.0, 1.0, 1.0, 1.0 }; // ���ʌ�

public:

    // �����ݒ�
    void InitLight()
    {
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

        glEnable(GL_LIGHT0);
    }

    Light() { }
};