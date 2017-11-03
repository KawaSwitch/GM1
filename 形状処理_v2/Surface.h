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

    // ����_�ݒ�
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

        _ctrlp.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);
    }

    // �x�N�g���擾�֐�
    virtual Vector3d GetPositionVector(double u, double v) { return Vector3d(); }; // �ʒu�x�N�g��
    virtual Vector3d GetFirstDiffVector(double u, double v) { return Vector3d(); }; // �ڐ��x�N�g��
    virtual Vector3d GetSecondDiffVector(double u, double v) { return Vector3d(); }; // 2�K�����x�N�g��
    virtual Vector3d GetCurvatureVector(double u, double v) { return Vector3d(); }; // �ȗ��x�N�g��

public:

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

    ~Surface() { }
};