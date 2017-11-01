#pragma once
#pragma comment(lib,"glew32.lib")

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
    bool _isSelected = false; // �I����Ԃɂ��邩
    unsigned int _number; // ���ʔԍ�

    // �f�B�X�v���C���X�g�ݒ�
    void SetDisplayList()
    {
        _displayList = glGenLists(1);

        if (_displayList == 0)
            Error::ShowAndExit("�f�B�X�v���C���X�g�쐬���s");
    }

protected:

    bool _isUseVBO = false; // VBO���g����
    GLuint _vbo;
    GLdouble _color[4];  // �F

    // ���O�`��
    virtual void PreDraw() = 0;
    virtual void CreateVBO() { };
    virtual void ModifyVBO() { };
    virtual void DrawVBO() { };

    bool _isDrawCtrlp = false;

public:

    // �`��
    virtual void Draw()
    {
        // VBO
        if (_isUseVBO)
        {
            if (!_isRendered)
            {
                CreateVBO(); // VBO�쐬

                _isRendered = true;
                glutPostRedisplay();
            }
            else
            {
                glColor3dv(_color); // �F�w��
                DrawVBO(); // �`��
            }

        }
        // �f�B�X�v���C���X�g
        else
        {
            if (!_isRendered)
            {
                SetDisplayList();

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
    }

    // ���̂܂ܕ`��
    void DrawAsItIs()
    {
        this->PreDraw();
    }

    // �F�ݒ�
    void SetColor(GLdouble* color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
    }

    // �I����ԃg�O��
    void SetUnsetSelected()
    {
        _isSelected = !_isSelected;
    }

    // ����_���`��
    virtual void DrawControlPointsAndLines() = 0;

    // ����_�\���g�O��
    void SetUnsetIsDrawCtrlp()
    {
        _isDrawCtrlp = !_isDrawCtrlp;
    }

    // �I�u�W�F�N�g�ԍ����擾
    unsigned int GetObjectNumber()
    {
        return _number;
    }

    // delete�t���O
    void RaiseDeleteFlag() { _deleteFlag = true; }
    bool IsDeleteFlagRaised() { return _deleteFlag == true; }

public:

    Object()
    {
        _isRendered = false;
        _deleteFlag = false;
        _number = obj_number++; // ���ʎq��U��

        _color[0] = -1;
    }

    virtual ~Object()
    {
        obj_number--; // ���ʎq���󂯂�
        glDeleteLists(_displayList, 1);
    }
};
