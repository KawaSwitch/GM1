#pragma once

#include "GV.h"
#include "ControlPoint.h"

// �Ȑ��C���^�[�t�F�[�X
class Curve : public Object
{
protected:

    int _ord; // �K��
    int _ncpnt; // ����_��
    vector<ControlPoint> _ctrlp; // ����_
    int _ctrlp_displayList = -1; // ����_�p�f�B�X�v���C���X�g

    // ����_�ݒ�
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

        _ctrlp.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);
    }

    // ����_���`��
    virtual void DrawControlPointsAndLines() override
    {
        if (_isDrawCtrlp)
        {
            if (_ctrlp_displayList == -1)
            {
                if (!(_ctrlp_displayList = glGenLists(1)))
                    return;

                // �o�^
                glNewList(_ctrlp_displayList, GL_COMPILE);
                DrawCPsInternal();
                glEndList();

                glutPostRedisplay();
            }
            else
                glCallList(_ctrlp_displayList);
        }
    }

    virtual Vector3d GetPositionVector(double t) = 0; // �ʒu�x�N�g��

private:

    void DrawCPsInternal()
    {
        glColor3d(1.0, 0.0, 0.0); // ��
        glPointSize(5.0);
        glLineWidth(1.0);

        // �_�Q
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();

        // ���Q
        glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();
    }

public:

    virtual ~Curve() { };
};