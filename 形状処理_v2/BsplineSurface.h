#pragma once

#include "Surface.h"

class BsplineSurface : public Surface
{
protected:

    int _nknotU, _nknotV; // �m�b�g�x�N�g���T�C�Y
    vector<double> _knotU, _knotV; // �m�b�g�x�N�g��

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
    void CreateIBO() override;
    void DrawIBO() override;

    // �m�b�g�x�N�g���ݒ�
    void SetKnotVector(double* knot, int size, vector<double>& _knot);

private:

    // �w�肵���[�̋Ȑ����擾����
    Curve* GetEdgeCurve(SurfaceEdge edge) override;

    // �w��p�����[�^�̃x�N�g�������֐�����Z�o����
    Vector3d CalcVector(
        double u, double v,
        function<double(unsigned, unsigned, double, double *)> BasisFuncU,
        function<double(unsigned, unsigned, double, double *)> BasisFuncV);

public:

    BsplineSurface(int u_mord, int v_mord, ControlPoint* cp, int u_cp_size, int v_cp_size, double* u_knot, double* v_knot, GLdouble* color, GLdouble width);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double u, double v) override;
    Vector3d GetFirstDiffVectorU(double u, double v) override;
    Vector3d GetFirstDiffVectorV(double u, double v) override;
    Vector3d GetSecondDiffVectorUU(double u, double v) override;
    Vector3d GetSecondDiffVectorUV(double u, double v) override;
    Vector3d GetSecondDiffVectorVV(double u, double v) override;
};
