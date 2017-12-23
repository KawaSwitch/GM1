#pragma once

#include "Curve.h"

// B�X�v���C���Ȑ�
class BsplineCurve : public Curve
{
protected:

    int _nknot; // �m�b�g�x�N�g���T�C�Y
    vector<double> _knot; // �m�b�g�x�N�g��

    // �e��x�N�g���`��
    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    void DrawNormalVectorsInternal() override;
    void DrawCurvatureVectorsInternal() override;

    // ���O�`��
    void PreDraw() override;

    // �o�b�t�@�I�u�W�F�N�g
    void CreateVBO() override;
    void DrawVBO() override;

    // �m�b�g�x�N�g���ݒ�
    void SetKnotVector(double* knot, int size);

public:

    BsplineCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double t) override;
    Vector3d GetFirstDiffVector(double t) override;
    Vector3d GetSecondDiffVector(double t) override;

    // �t�ϊ�
    Curve* GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width) override;
};
