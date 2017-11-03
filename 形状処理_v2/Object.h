#pragma once
#pragma comment(lib,"glew32.lib")

#include "GV.h"
#include "Scene.h"
#include "Error.h"
#include "ControlPoint.h"
#include "Box.h"

class Scene;

// �I�u�W�F�N�g���N���X
class Object
{
private:

    bool _deleteFlag = false;
    bool _isSelected = false; // �I����Ԃɂ��邩
    unsigned int _number; // ���ʔԍ�

protected:

    bool _isUseVBO = false; // VBO���g����
    GLuint _vbo = 0;
    GLdouble _color[4];  // �F
    vector<ControlPoint> _ctrlp; // ����_

    // ���O�`��
    virtual void PreDraw() = 0;
    virtual void DrawCPsInternal() = 0;
    virtual void DrawFirstDiffVectorsInternal() { };
    virtual void DrawSecondDiffVectorsInternal() { };
    virtual void DrawCurvatureVectorsInternal() { };

    // VBO
    virtual void CreateVBO() { };
    virtual void ModifyVBO() { };
    virtual void DrawVBO() { };
    
    // �f�B�X�v���C���X�g
    int _displayList = -1; // �I�u�W�F�N�g�p
    int _ctrlp_displayList = -1;
    int _fd_displayList = -1;
    int _sd_displayList = -1;
    int _box_displayList = -1;
    int _cur_displayList = -1;

    // �\���n�u�[���A��
    bool _isDrawCtrlp = true; // ����_
    bool _isDrawFirstDiff = false; // �ڐ�
    bool _isDrawSecondDiff = false; // 2�K����
    bool _isDrawBox = false; // �~�j�}�N�X�{�b�N�X
    bool _isDrawCurvature = false; // �ȗ�

public:

    // �I�u�W�F�N�g�`��
    virtual void Draw()
    {
        // VBO
        if (_isUseVBO)
        {
            if (_vbo == 0)
            {
                CreateVBO(); // VBO�쐬
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
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
    }

    // ���̂܂ܕ`��
    void DrawAsItIs()
    {
        this->PreDraw();
    }

    // �~�j�}�N�X�{�b�N�X�`��
    void DrawBoxInternal()
    {
        Box box(_ctrlp);
        box.Draw(Color::orange, 1.0);
    }

    // �g�O���`�F���W
    void SetUnsetSelected() { _isSelected = !_isSelected; }
    void SetUnsetIsDrawCtrlp() { _isDrawCtrlp = !_isDrawCtrlp; }
    void SetUnsetIsDrawFisrtDiff() { _isDrawFirstDiff = !_isDrawFirstDiff; }
    void SetUnsetIsDrawSecondDiff() { _isDrawSecondDiff = !_isDrawSecondDiff; }
    void SetUnsetIsDrawBox() { _isDrawBox = !_isDrawBox; }
    void SetUnsetIsDrawCurvature() { _isDrawCurvature = !_isDrawCurvature; }

    // ����_���`��
    void DrawControlPointsAndLines()
    {
        if (_isDrawCtrlp)
            DrawUsingDisplayList(&_ctrlp_displayList, [&] { return (*this).DrawCPsInternal(); });
    }
    // �ڐ��x�N�g���`��
    void DrawFirstDiffVectors()
    {
        if (_isDrawFirstDiff)
            DrawUsingDisplayList(&_fd_displayList, [&] { return (*this).DrawFirstDiffVectorsInternal(); });
    }
    // 2�K�����x�N�g���`��
    void DrawSecondDiffVectors()
    {
        if (_isDrawSecondDiff)
            DrawUsingDisplayList(&_sd_displayList, [&] { return (*this).DrawSecondDiffVectorsInternal(); });
    }
    // �~�j�}�N�X�{�b�N�X�`��
    void DrawBox()
    {
        if (_isDrawBox)
            DrawUsingDisplayList(&_box_displayList, [&] { return (*this).DrawBoxInternal(); });
    }
    // �ȗ��x�N�g���`��
    void DrawCurvatureVectors()
    {
        if (_isDrawCurvature)
            DrawUsingDisplayList(&_cur_displayList, [&] { return (*this).DrawCurvatureVectorsInternal(); });
    }

    // �F�ݒ�
    void SetColor(GLdouble* color)
    {
        for (int i = 0; i < 4; i++)
            _color[i] = color[i];
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
        _number = obj_number++; // ���ʎq��U��

        _vbo = 0;
        _color[0] = -1;
    }

    virtual ~Object()
    {
        obj_number--; // ���ʎq���󂯂�

        // �f�B�X�v���C���X�g�j��
        glDeleteLists(_displayList, 1);
        glDeleteLists(_ctrlp_displayList, 1);
        glDeleteLists(_fd_displayList, 1);
        glDeleteLists(_sd_displayList, 1);
        glDeleteLists(_box_displayList, 1);
        glDeleteLists(_cur_displayList, 1);

        // VBO�j��
        glDeleteBuffers(1, &_vbo);
    }
};
