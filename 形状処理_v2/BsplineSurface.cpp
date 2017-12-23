#include "BsplineSurface.h"
#include "BsplineCurve.h"

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
    _min_draw_param_U = u_knot[_ordU - 1];  _max_draw_param_U = u_knot[_ncpntU];
    _min_draw_param_V = v_knot[_ordV - 1];  _max_draw_param_V = v_knot[_ncpntV];

    SetControlPoint(cp, u_cp_size * v_cp_size);
    SetKnotVector(u_knot, _nknotU, _knotU);
    SetKnotVector(v_knot, _nknotV, _knotV);
    SetColor(color);
    _mesh_width = width;

    // VBO���g��
    _isUseVBO = true;
    // IBO���g��
    //_isUseIBO = true;
}

// ���O�`��
void BsplineSurface::PreDraw()
{   
    // �𑜓x
    int RES = 15;

    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;

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

    nor.resize(u_size);
    for (int i = 0; i < u_size; i++)
        nor[i].resize(v_size);

    for (int i = u_min; i <= u_max; i++)
    {
        for (int j = v_min; j <= v_max; j++)
        {
            double u = (double)i / RES;
            double v = (double)j / RES;

            pnt[i - u_min][j - v_min] = GetPositionVector(u, v);
            nor[i - u_min][j - v_min] = GetNormalVector(u, v).Normalize();
        }
    }

    //glColor4dv(_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _color);

    // �O�p�|���S���\��
    for (int i = u_min; i < u_max; i++)
    {
        for (int j = v_min; j < v_max; j++)
        {
            glBegin(GL_TRIANGLES);

            // ��O�p
            glNormal3d(nor[i - u_min][j - v_min]);
            glVertex3d(pnt[i - u_min][j - v_min]);

            glNormal3d(nor[i + 1 - u_min][j - v_min]);
            glVertex3d(pnt[i + 1 - u_min][j - v_min]);

            glNormal3d(nor[i - u_min][j + 1 - v_min]);
            glVertex3d(pnt[i - u_min][j + 1 - v_min]);
            
            // ���O�p
            glNormal3d(nor[i + 1 - u_min][j - v_min]);
            glVertex3d(pnt[i + 1 - u_min][j - v_min]);
            
            glNormal3d(nor[i + 1 - u_min][j + 1 - v_min]);
            glVertex3d(pnt[i + 1 - u_min][j + 1 - v_min]);

            glNormal3d(nor[i - u_min][j + 1 - v_min]);
            glVertex3d(pnt[i - u_min][j + 1 - v_min]);

            glEnd();
        }
    };
}

// ���b�V���`��
void BsplineSurface::DrawMeshInternal()
{
    Vector3d pnt;

    // ���b�V���̐F��3dv�œn���������Y��(��=0)
    glColor3dv(_color);
    glLineWidth(_mesh_width);

    // U����
    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(u, v);
            glVertex3d(pnt);
        }

        glEnd();
    }
    // V����
    for (int i = (int)(_knotV[_ordV - 1] * 100); i <= (int)(_knotV[_ncpntV] * 100); i += 10)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = (int)(_knotU[_ordU - 1] * 100); j <= (int)(_knotU[_ncpntU] * 100); j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(v, u);
            glVertex3d(pnt);
        }

        glEnd();
    }
}

// ���_�o�b�t�@(VBO)�쐬
void BsplineSurface::CreateVBO()
{
    // �𑜓x
    int RES = 15;

    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;
    vector<Vector3d> pnt_vbo;
    vector<Vector3d> nor_vbo;

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

    nor.resize(u_size);
    for (int i = 0; i < u_size; i++)
        nor[i].resize(v_size);

    for (int i = u_min; i <= u_max; i++)
    {
        for (int j = v_min; j <= v_max; j++)
        {
            double u = (double)i / RES;
            double v = (double)j / RES;

            pnt[i - u_min][j - v_min] = GetPositionVector(u, v);
            nor[i - u_min][j - v_min] = GetNormalVector(u, v).Normalize();
        }
    }

    // VBO�p�̒��_�擾
    for (int i = u_min; i < u_max; i++)
    {
        for (int j = v_min; j < v_max; j++)
        {
            pnt_vbo.push_back(pnt[i - u_min][j - v_min]);
            pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            pnt_vbo.push_back(pnt[i + 1 - u_min][j + 1 - v_min]);
            pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            // �@��
            nor_vbo.push_back(nor[i - u_min][j - v_min]);
            nor_vbo.push_back(nor[i + 1 - u_min][j - v_min]);
            nor_vbo.push_back(nor[i - u_min][j + 1 - v_min]);

            nor_vbo.push_back(nor[i + 1 - u_min][j - v_min]);
            nor_vbo.push_back(nor[i + 1 - u_min][j + 1 - v_min]);
            nor_vbo.push_back(nor[i - u_min][j + 1 - v_min]);
        }
    }

    _nVertex = (int)pnt_vbo.size();

    // VBO�̐ݒ�
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_vbo_nor);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &nor_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// VBO�ŕ`��
void BsplineSurface::DrawVBO()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _color);

    // ���_
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    // �@��
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, (void *)0);

    // �`��
    glDrawArrays(GL_TRIANGLES, 0, _nVertex);

    // clean up
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ���_�o�b�t�@(IBO)�쐬
void BsplineSurface::CreateIBO()
{
    // �𑜓x
    int RES = 15;

    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;
    vector<Vector3d> pnt_vbo;
    vector<Vector3d> nor_vbo;
    vector<GLushort> pnt_ibo;

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

    nor.resize(u_size);
    for (int i = 0; i < u_size; i++)
        nor[i].resize(v_size);

    for (int i = u_min; i <= u_max; i++)
    {
        for (int j = v_min; j <= v_max; j++)
        {
            double u = (double)i / RES;
            double v = (double)j / RES;

            pnt[i - u_min][j - v_min] = GetPositionVector(u, v);
            nor[i - u_min][j - v_min] = GetNormalVector(u, v).Normalize();
        }
    }

    // IBO�p�̒��_�擾
    for (int i = u_min; i < u_max; i++)
    {
        for (int j = v_min; j < v_max; j++)
        {
            //pnt_ibo.push

            //pnt_vbo.push_back(pnt[i - u_min][j - v_min]);
            //pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            //pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            //pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            //pnt_vbo.push_back(pnt[i + 1 - u_min][j + 1 - v_min]);
            //pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            //// �@��
            //nor_vbo.push_back(nor[i - u_min][j - v_min]);
            //nor_vbo.push_back(nor[i + 1 - u_min][j - v_min]);
            //nor_vbo.push_back(nor[i - u_min][j + 1 - v_min]);

            //nor_vbo.push_back(nor[i + 1 - u_min][j - v_min]);
            //nor_vbo.push_back(nor[i + 1 - u_min][j + 1 - v_min]);
            //nor_vbo.push_back(nor[i - u_min][j + 1 - v_min]);
        }
    }

    _nVertex = (int)pnt_vbo.size();

    // VBO�̐ݒ�
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // IBO�̐ݒ�
    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glGenBuffers(1, &_vbo_nor);
    //glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &nor_vbo[0], GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// IBO�ŕ`��
void BsplineSurface::DrawIBO()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _color);

    // ���_
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_DOUBLE, 0, 0);

    //// �@��
    //glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    //glEnableClientState(GL_NORMAL_ARRAY);
    //glNormalPointer(GL_DOUBLE, 0, (void *)0);

    // �`��
    glDrawElements(GL_TRIANGLES, _nVertex, GL_UNSIGNED_INT, (void *)0);

    // clean up
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// �ڐ��x�N�g���`��
void BsplineSurface::DrawFirstDiffVectorsInternal()
{
    Vector3d pnt, diff;
    glLineWidth(2.0);

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

// 2�K�����x�N�g���`��
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
            glColor3d(1.0, 0.2, 0.2); // ��
            pnt = GetPositionVector(u, v);
            diff = GetSecondDiffVectorUU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // UV����
            glColor3d(0.2, 1.0, 0.2); // ��
            diff = GetSecondDiffVectorUV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // VV����
            glColor3d(0.2, 0.2, 1.0); // ��
            diff = GetSecondDiffVectorVV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// �@���x�N�g���`��
void BsplineSurface::DrawNormalVectorsInternal()
{
    Vector3d pnt, normal;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            // �@��
            glColor3dv(Color::blue); // ��
            pnt = GetPositionVector(u, v);
            normal = GetNormalVector(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + normal);
        }
    }

    glEnd();
}

// �ȗ����a�`��
void BsplineSurface::DrawCurvatureVectorsInternal()
{
    Vector3d pnt, curv;

    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = (int)(_knotU[_ordU - 1] * 100); i <= (int)(_knotU[_ncpntU] * 100); i += 10)
    {
        for (int j = (int)(_knotV[_ordV - 1] * 100); j <= (int)(_knotV[_ncpntV] * 100); j += 10)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(u, v);
            curv = GetCurvatureVector(u, v);

            // �ȗ����a
            glColor3dv(Color::pink); // �s���N
            glBegin(GL_LINES);
            glVertex3d(pnt);
            glVertex3d(pnt + curv);
            glEnd();

            // �ȗ����S
            glColor3dv(Color::light_green); // ����
            glBegin(GL_POINTS);
            glVertex3d(pnt + curv);
            glEnd();
        }
    }
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

// �w�肵���[�̋Ȑ����擾����
Curve* BsplineSurface::GetEdgeCurve(SurfaceEdge edge)
{
    vector<ControlPoint> edge_cp = GetEdgeCurveControlPoint(edge);
    int edge_ord = (edge == U_min || edge == U_max) ? _ordV : _ordU;
    vector<double> edge_knot = (edge == U_min || edge == U_max) ? _knotV : _knotU;

    return new BsplineCurve(edge_ord, &edge_cp[0], (int)edge_cp.size(), &edge_knot[0], Color::red, _mesh_width);
}