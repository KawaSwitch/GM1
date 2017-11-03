#include "BsplineSurface.h"

BsplineSurface::BsplineSurface(
    int u_mord, int v_mord, ControlPoint* cp, 
    int u_cp_size, int v_cp_size, double* u_knot, double* v_knot,
    GLdouble* color, GLdouble width)
{
    _ordU = u_mord;
    _ordV = v_mord;
    _ncpntU = u_cp_size;
    _ncpntV = v_cp_size;
    _nknotU = u_mord + u_cp_size;
    _nknotV = v_mord + v_cp_size;

    SetControlPoint(cp, u_cp_size * v_cp_size);
    SetKnotVector(u_knot, _nknotU, _knotU);
    SetKnotVector(v_knot, _nknotV, _knotV);
}

// ���O�`��
void BsplineSurface::PreDraw()
{

}

// ���_�o�b�t�@�쐬
void BsplineSurface::CreateVBO()
{

}

// VBO�ŕ`��
void BsplineSurface::DrawVBO()
{

}

// �ڐ��x�N�g���`��
void BsplineSurface::DrawFirstDiffVectorsInternal()
{

}

// �ʒu�x�N�g���擾
Vector3d BsplineSurface::GetPositionVector(double u, double v)
{
    Vector3d pnt;



    return pnt;
}

// �ڐ��x�N�g���擾
Vector3d BsplineSurface::GetFirstDiffVector(double u, double v)
{
    Vector3d diff;



    return diff;
}