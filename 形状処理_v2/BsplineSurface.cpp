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
    glDepthMask(GL_FALSE);

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
    glColor4dv(_color);

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

    glDepthMask(GL_TRUE);
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
    Vector3d pnt, diff;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            // U����
            glColor3dv(Color::red); // ��
            pnt = GetPositionVector(u, v);
            diff = GetFirstDiffVectorU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // V����
            glColor3dv(Color::green); // ��
            diff = GetFirstDiffVectorV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// 2�K�����x�N�g���x�N�g���`��
void BsplineSurface::DrawSecondDiffVectorsInternal()
{
    Vector3d pnt, diff;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            // UU����
            glColor3dv(Color::blue); // ��
            pnt = GetPositionVector(u, v);
            diff = GetSecondDiffVectorUU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // UV����
            glColor3dv(Color::blue); // ��
            diff = GetSecondDiffVectorUV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // VV����
            glColor3dv(Color::blue); // ��
            diff = GetSecondDiffVectorVV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// �ʒu�x�N�g���擾
Vector3d BsplineSurface::GetPositionVector(double u, double v)
{
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

    double temp[100]; // �v�Z�p

    // ���֐��z��(�s��v�Z�p)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // ���֐��z��֊e���֐�����
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc1DiffBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);

    // �ʒu�x�N�g���Z�o(�s��v�Z)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BsplineSurface::GetFirstDiffVectorV(double u, double v)
{
    Vector3d diff;

    double temp[100]; // �v�Z�p

    // ���֐��z��(�s��v�Z�p)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // ���֐��z��֊e���֐�����
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc1DiffBsplineFunc(i, _ordV, v, &_knotV[0]);

    // �ʒu�x�N�g���Z�o(�s��v�Z)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}

// 2�K�����x�N�g���擾
Vector3d BsplineSurface::GetSecondDiffVectorUU(double u, double v)
{
    Vector3d diff;

    double temp[100]; // �v�Z�p

    // ���֐��z��(�s��v�Z�p)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // ���֐��z��֊e���֐�����
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc2DiffBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBsplineFunc(i, _ordV, v, &_knotV[0]);

    // �ʒu�x�N�g���Z�o(�s��v�Z)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BsplineSurface::GetSecondDiffVectorUV(double u, double v)
{
    Vector3d diff;

    double temp[100]; // �v�Z�p

    // ���֐��z��(�s��v�Z�p)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // ���֐��z��֊e���֐�����
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc1DiffBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc1DiffBsplineFunc(i, _ordV, v, &_knotV[0]);

    // �ʒu�x�N�g���Z�o(�s��v�Z)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BsplineSurface::GetSecondDiffVectorVV(double u, double v)
{
    Vector3d diff;

    double temp[100]; // �v�Z�p

    // ���֐��z��(�s��v�Z�p)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // ���֐��z��֊e���֐�����
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBsplineFunc(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc2DiffBsplineFunc(i, _ordV, v, &_knotV[0]);

    // �ʒu�x�N�g���Z�o(�s��v�Z)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}