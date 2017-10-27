#pragma once

#include "GV.h"
#include "Scene.h"
#include "Error.h"

class Scene;

// �I�u�W�F�N�g���N���X
class Object
{
private:

    int _displayList;    // �f�B�X�v���C���X�g
    bool _isRendered;
    bool _deleteFlag;

    // �f�B�X�v���C���X�g�ݒ�
    void SetDisplayList()
    {
        _displayList = glGenLists(1);

        if (_displayList == 0)
            Error::ShowAndExit("�f�B�X�v���C���X�g�쐬���s");
    }

protected:

    GLdouble _color[4];  // �F

    // ���O�`��
    virtual void PreDraw() = 0;

public:

    // �`��
    virtual void Draw()
    {
        if (!_isRendered)
        {
            // �o�^
            glNewList(_displayList, GL_COMPILE);
            this->PreDraw();
            glEndList();

            _isRendered = true;

            glutPostRedisplay();
        }
        else
        {
            glCallList(_displayList);
        }
    }

    // �F�ݒ�
    void SetColor(GLdouble* color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
    }

    // delete�t���O
    void RaiseDeleteFlag() { _deleteFlag = true; }
    bool IsDeleteFlagRaised() { return _deleteFlag == true; }

public:

    Object()
    {
        this->SetDisplayList();
        _isRendered = false;
        _deleteFlag = false;

        _color[0] = -1;
    }

    virtual ~Object()
    {
        glDeleteLists(_displayList, 1);
    }
};
