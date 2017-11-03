#pragma once

#include "Curve.h"

// B�X�v���C���Ȑ�
class BsplineCurve : public Curve
{
private:

    int _nknot; // �m�b�g�x�N�g���T�C�Y
    vector<double> _knot; // �m�b�g�x�N�g��
    int _nVertex; // ���_��

    void DrawFirstDiffVectorsInternal() override;
    void DrawSecondDiffVectorsInternal() override;
    //void DrawBoxInternal() override;
    //void DrawCurvatureVectorsInternal() override;

    // ���O�`��
    void PreDraw() override;
    void CreateVBO() override;
    void DrawVBO() override;

    // �m�b�g�x�N�g���ݒ�
    void SetKnotVector(double* knot, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("�m�b�g�x�N�g���ݒ莸�s", "knot-vector size must be over 0.");

        _knot.reserve(size);
        for (int i = 0; i < size; i++)
            _knot.emplace_back(knot[i]);
    }

public:

    BsplineCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width);

    Vector3d GetPositionVector(double t) override;
    Vector3d GetFirstDiffVector(double t) override;
    Vector3d GetSecondDiffVector(double t) override;
};
