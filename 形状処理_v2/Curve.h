#pragma once

#include "Object.h"
#include "ControlPoint.h"

// �Ȑ��C���^�[�t�F�[�X
class Curve : public Object
{
protected:

    int _ord; // �K��
    int _ncpnt; // ����_��
    int _nVertex; // ���_��
    double _width; // �Ȑ��̕�
    double _min_draw_param, _max_draw_param; // �`��͈̓p�����[�^

    // �x�N�g���擾�֐�
    virtual Vector3d GetPositionVector(double t) = 0; // �ʒu�x�N�g��
    virtual Vector3d GetFirstDiffVector(double t) = 0; // �ڐ��x�N�g��
    virtual Vector3d GetSecondDiffVector(double t) = 0; // 2�K�����x�N�g��

    // �@���x�N�g���擾
    Vector3d GetNormalVector(double t) { return (Vector3d(0, 0, 1) * GetFirstDiffVector(t)); }

    // �ȗ��x�N�g���擾
    Vector3d GetCurvatureVector(double t);

    // ����_�ݒ�
    void SetControlPoint(ControlPoint* cp, int size);

private:

    // ����_�`��
    void DrawCPsInternal() override;

public:

    // �Q�Ɠ_����̍ŋߓ_���擾����
    Vector3d GetNearestPointFromRef(Vector3d ref);

    // �`��͈͂�split_num�ɕ�������悤�Ȉʒu�x�N�g�����擾����
    vector<Vector3d> GetPositionVectors(int split_num);

    // �ʉߓ_����t�ϊ����ċȐ����擾����
    virtual Curve* GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width) = 0;

    virtual ~Curve() { };
};