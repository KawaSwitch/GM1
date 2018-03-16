#pragma once

#include "GV.h"

// �����N���X
// ���C���ɂ����g���\��Ȃ��̂œK��
class Light
{
private:

    GLenum lightNum;

    // �K��
    GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 }; // �g�U��
    GLfloat lightAmbient[4] = { 1.0, 1.0, 1.0, 1.0 }; // ����
    GLfloat lightSpecular[4] = { 0.1f, 0.1f, 0.1f, 1.0 }; // ���ʌ�
    GLfloat* lightPosition; // �ʒu

public:

    // �����I��
    void On()
    {
        glEnable(lightNum);
    }

    // �����I�t
    void Off()
    {
        glDisable(lightNum);
    }

    Light(GLenum light, double posX, double posY, double posZ) 
    {
        lightNum = light;

        GLfloat pos[4] = { (GLfloat)posX, (GLfloat)posY, (GLfloat)posZ, 1.0 };
        lightPosition = pos;

        glLightfv(lightNum, GL_POSITION, lightPosition);
        glLightfv(lightNum, GL_DIFFUSE, lightDiffuse);
        glLightfv(lightNum, GL_AMBIENT, lightAmbient);
        glLightfv(lightNum, GL_SPECULAR, lightSpecular);
    }
};