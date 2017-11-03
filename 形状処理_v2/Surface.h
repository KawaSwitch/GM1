#pragma once

#include "Object.h"
#include "ControlPoint.h"

// �ȖʃC���^�[�t�F�[�X
class Surface : public Object
{
protected:

    int _ordU, _ordV; // �K��
    int _ncpntU, _ncpntV; // ����_��
    vector<ControlPoint> _ctrlp; // ����_
    vector<double> _ctrlpX; // �v�Z�p
    vector<double> _ctrlpY;
    vector<double> _ctrlpZ;
    int _mesh_displayList = -1; // ���b�V���p�f�B�X�v���C���X�g

    // ����_�ݒ�
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

        _ctrlp.reserve(size); _ctrlpX.reserve(size);
        _ctrlpY.reserve(size); _ctrlpZ.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);

        // �v�Z�p�ɓ]�u����(�����ƌ��h���������������~����)
        for (int i = 0; i < _ncpntU; i++)
        {
            for (int j = 0; j < _ncpntV; j++)
            {
                _ctrlpX.push_back(_ctrlp[j * _ncpntU + i].X);
                _ctrlpY.push_back(_ctrlp[j * _ncpntU + i].Y);
                _ctrlpZ.push_back(_ctrlp[j * _ncpntU + i].Z);
            }
        }
    }

    // ���b�V���\��
    virtual void DrawMeshInternal() { };

    void DrawMesh()
    {
        // ���b�V���͐�Ε\��(���h���̂���)
        DrawUsingDisplayList(&_mesh_displayList, [&] { return (*this).DrawMeshInternal(); });
    }

    // �x�N�g���擾�֐�
    virtual Vector3d GetPositionVector(double u, double v) = 0; // �ʒu�x�N�g��
    virtual Vector3d GetFirstDiffVectorU(double u, double v) = 0; // �ڐ��x�N�g��
    virtual Vector3d GetFirstDiffVectorV(double u, double v) = 0;
    virtual Vector3d GetSecondDiffVectorUU(double u, double v) { return Vector3d(); }; // 2�K�����x�N�g��
    virtual Vector3d GetSecondDiffVectorUV(double u, double v) { return Vector3d(); };
    virtual Vector3d GetSecondDiffVectorVV(double u, double v) { return Vector3d(); };
    virtual Vector3d GetCurvatureVector(double u, double v) { return Vector3d(); }; // �ȗ��x�N�g��

public:

    // �I�u�W�F�N�g�`��
    void Draw() override
    {
        // �I�u�W�F�N�g���g�̓f�v�X�l������(���h���̂���)
        glDepthMask(GL_FALSE);

        if (_isUseVBO)
        {
            // VBO
            if (_vbo == 0)
            {
                CreateVBO(); // VBO�쐬
                glutPostRedisplay();
            }
            else
                DrawVBO(); // �`��
        }
        else
        {
            // �f�B�X�v���C���X�g
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
        }

        glDepthMask(GL_TRUE);

        // ���b�V���̓f�B�X�v���C���X�g
        DrawMesh();
    }

    // ����_�`��
    void DrawCPsInternal() override
    {
        glColor3d(1.0, 0.0, 0.0); // ��
        glPointSize(5.0);
        glLineWidth(1.0);

        // �_�Q
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();

        // U�������Q
        for (int i = 0; i < _ncpntV; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < _ncpntU; j++)
                glVertex3d(_ctrlp[i * _ncpntU + j].X, _ctrlp[i * _ncpntU + j].Y, _ctrlp[i * _ncpntU + j].Z);
            glEnd();
        }
        // V�������Q
        for (int i = 0; i < _ncpntU; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < _ncpntV; j++)
                glVertex3d(_ctrlp[j * _ncpntU + i].X, _ctrlp[j * _ncpntU + i].Y, _ctrlp[j * _ncpntU + i].Z);
            glEnd();
        }
    }

    virtual ~Surface() { glDeleteLists(_mesh_displayList, 1); }
};