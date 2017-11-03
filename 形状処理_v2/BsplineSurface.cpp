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
    SetColor(color);
}

// ���O�`��
void BsplineSurface::PreDraw()
{   
    // �𑜓x
    int RES = 15;

    vector<vector<Vector3d>> pnt;

    // �`��͈͂�\�ߐݒ�
    int u_min = (int)(_knotU[_ordU - 1] * RES);
    int v_min = (int)(_knotV[_ordV - 1] * RES);
    int u_max = (int)(_knotU[_ncpntU] * RES);
    int v_max = (int)(_knotV[_ncpntV] * RES);
    int u_size = u_max - u_min + 1;
    int v_size = v_max - v_min + 1;

    pnt.resize(u_size);
    for (int i = 0; i < u_size; i++)
        pnt[i].resize(v_size);

    for (int i = u_min; i <= u_max; i++)
    {
        for (int j = v_min; j <= v_max; j++)
        {
            double u = (double)i / RES;
            double v = (double)j / RES;

            pnt[i - u_min][j - v_min] = GetPositionVector(u, v);
        }
    }

    // ���b�V���̐F�̓��l�֌W�Ȃ��̂ŋC�ɂ������v
    glColor3dv(_color);

    // ���b�V���\��
    // U����
    for (int i = u_min; i <= u_max; i++)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = v_min; j <= v_max; j++)
        {
            glVertex3d(pnt[i - u_min][j - v_min]);
        }

        glEnd();
    }
    // V����
    for (int i = v_min; i <= v_max; i++)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = u_min; j <= u_max; j++)
        {
            glVertex3d(pnt[j - u_min][i - v_min]);
        }

        glEnd();
    }

    // �O�p�|���S���\��
    for (int i = u_min; i < u_max; i++)
    {
        for (int j = v_min; j < v_max; j++)
        {
            glBegin(GL_TRIANGLE_STRIP);
            glVertex3d(pnt[i - u_min][j - v_min]);
            glVertex3d(pnt[i + 1 - u_min][j - v_min]);
            glVertex3d(pnt[i - u_min][j + 1 - v_min]);
            glVertex3d(pnt[i + 1 - u_min][j + 1 - v_min]);
            glEnd();
        }
    }
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
    using namespace cpplinq;

    Vector3d pnt;
    double temp[100]; // �v�Z�p

    // ���֐��z��(�s��v�Z�p)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // ���֐��z��֊e���֐�����
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);

    // �ʒu�x�N�g���Z�o(�s��v�Z)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    pnt.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    pnt.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    pnt.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return pnt;
}

// �ڐ��x�N�g���擾
Vector3d BsplineSurface::GetFirstDiffVectorU(double u, double v)
{
    Vector3d diff;



    return diff;
}
Vector3d BsplineSurface::GetFirstDiffVectorV(double u, double v)
{
    Vector3d diff;



    return diff;
}