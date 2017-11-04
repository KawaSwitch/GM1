#pragma once

#include "Surface.h"

class BsplineSurface : public Surface
{
private:

    int _nknotU, _nknotV; // �m�b�g�x�N�g���T�C�Y
    vector<double> _knotU, _knotV; // �m�b�g�x�N�g��

    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    void DrawNormalVectorsInternal() override;
    void DrawCurvatureVectorsInternal() override;
    void DrawMeshInternal() override;

    // ���O�`��
    void PreDraw() override;
    void CreateVBO() override;
    void DrawVBO() override;

    // �m�b�g�x�N�g���ݒ�
    void SetKnotVector(double* knot, int size, vector<double>& _knot)
    {
        if (size <= 0)
            Error::ShowAndExit("�m�b�g�x�N�g���ݒ莸�s", "knot-vector size must be over 0.");

        _knot.reserve(size);
        for (int i = 0; i < size; i++)
            _knot.emplace_back(knot[i]);
    }

public:

    BsplineSurface(int u_mord, int v_mord, ControlPoint* cp, int u_cp_size, int v_cp_size, double* u_knot, double* v_knot, GLdouble* color, GLdouble width);

    Vector3d GetPositionVector(double u, double v) override;
    Vector3d GetFirstDiffVectorU(double u, double v) override;
    Vector3d GetFirstDiffVectorV(double u, double v) override;
    Vector3d GetSecondDiffVectorUU(double u, double v) override;
    Vector3d GetSecondDiffVectorUV(double u, double v) override;
    Vector3d GetSecondDiffVectorVV(double u, double v) override;
};
