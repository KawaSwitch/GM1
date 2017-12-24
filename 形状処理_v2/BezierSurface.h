#pragma once

#include "Surface.h"
#include "ControlPoint.h"

class BezierSurface : public Surface
{
private:

    // �e��x�N�g���`��
    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    void DrawNormalVectorsInternal() override;
    void DrawCurvatureVectorsInternal() override;

    // ���b�V���`��
    void DrawMeshInternal() override;

    // ���O�`��
    void PreDraw() override;

    // �o�b�t�@�I�u�W�F�N�g
    void CreateVBO() override;
    void DrawVBO() override;

    // �w�肵���[�̋Ȑ����擾����
    Curve* GetEdgeCurve(SurfaceEdge edge) override;

    // �w��p�����[�^�̃x�N�g�������֐�����Z�o����
    Vector3d CalcVector(
        double u, double v,
        function<double(unsigned, unsigned, double)> BasisFuncU,
        function<double(unsigned, unsigned, double)> BasisFuncV);

public:

    BezierSurface(int u_mord, int v_mord, ControlPoint* cp, int u_cp_size, int v_cp_size, GLdouble* color, GLdouble resol = 20);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double u, double v) override;
    Vector3d GetFirstDiffVectorU(double u, double v) override;
    Vector3d GetFirstDiffVectorV(double u, double v) override;
    Vector3d GetSecondDiffVectorUU(double u, double v) override;
    Vector3d GetSecondDiffVectorUV(double u, double v) override;
    Vector3d GetSecondDiffVectorVV(double u, double v) override;
};