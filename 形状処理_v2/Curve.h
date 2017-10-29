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

    // ����_�ݒ�
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

        _ctrlp.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);
    }

    virtual Vector3d GetPositionVector(double t) = 0; // �ʒu�x�N�g��


public:

    virtual ~Curve() { };
};