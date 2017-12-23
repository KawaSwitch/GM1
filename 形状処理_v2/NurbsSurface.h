#pragma once

#include "BsplineSurface.h"

class NurbsSurface : public BsplineSurface
{
private:

    // �w�肵���[�̋Ȑ����擾����
    Curve* GetEdgeCurve(SurfaceEdge edge) override;

public:

    NurbsSurface(int u_mord, int v_mord, ControlPoint* cp, int u_cp_size, int v_cp_size, double* u_knot, double* v_knot, GLdouble* color, GLdouble width);

    // �e��x�N�g���擾
    Vector3d GetPositionVector(double u, double v) override;
    Vector3d GetFirstDiffVectorU(double u, double v) override;
    Vector3d GetFirstDiffVectorV(double u, double v) override;
    Vector3d GetSecondDiffVectorUU(double u, double v) override;
    Vector3d GetSecondDiffVectorUV(double u, double v) override;
    Vector3d GetSecondDiffVectorVV(double u, double v) override;
};