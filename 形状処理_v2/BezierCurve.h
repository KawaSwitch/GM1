#pragma once

#include "Curve.h"

class BezierCurve : public Curve
{
private:

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

public:

    BezierCurve(int mord, ControlPoint* cp, int cp_size, GLdouble* color, GLdouble width = 1.0, double resol = 20);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(const double t) override;
    Vector3d GetFirstDiffVector(double t) override;
    Vector3d GetSecondDiffVector(double t) override;

    // �t�ϊ�
    Curve* GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width) override;
};