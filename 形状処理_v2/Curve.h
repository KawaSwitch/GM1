#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "NearestPointInfo.h"

// �Ȑ����N���X
class Curve : public Object
{
protected:

    int _ord; // �K��
    int _ncpnt; // ����_��
    double _width; // �Ȑ��̕�
    double _min_draw_param, _max_draw_param; // �`��͈̓p�����[�^

    // �o�b�t�@�I�u�W�F�N�g�p
    mutable int _nVertex_cache; // ���_��

    // �@���x�N�g���擾
    Vector3d GetNormalVector(double t) const { return (Vector3d(0, 0, 1) * GetFirstDiffVector(t)); }

    // �ȗ��x�N�g���擾
    Vector3d GetCurvatureVector(double t) const;

    // ����_�ݒ�
    void SetControlPoint(const ControlPoint* const cp, int size);

private:

    // ����_�`��
    void DrawCPsInternal() const override;

public:

    double GetMinDrawParam() const { return this->_min_draw_param; }
    double GetMaxDrawParam() const { return this->_max_draw_param; }

    // �x�N�g���擾�֐�
    virtual Vector3d GetPositionVector(double t) const = 0; // �ʒu�x�N�g��
    virtual Vector3d GetFirstDiffVector(double t) const = 0; // �ڐ��x�N�g��
    virtual Vector3d GetSecondDiffVector(double t) const = 0; // 2�K�����x�N�g��

    // �Q�Ɠ_����̍ŋߓ_���擾����
    virtual Vector3d GetNearestPointFromRef(const Vector3d& ref) const;

    // �Q�Ɠ_����̍ŋߓ_���擾����
    virtual NearestPointInfoC GetNearestPointInfoFromRef(const Vector3d& ref) const = 0;

    // �`��͈͂�split_num�ɕ�������悤�Ȉʒu�x�N�g�����擾����
    vector<Vector3d> GetPositionVectors(int split_num) const;

    // �ʉߓ_����t�ϊ����ċȐ����擾����
    virtual Curve* GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, GLdouble width) const = 0;

    // ���Ȑ��Ƃ̑���x���v�Z���܂�
    double CalcDifferency(const Curve* const other) const;

    virtual ~Curve() { };
};