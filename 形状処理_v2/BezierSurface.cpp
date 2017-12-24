#include "BezierSurface.h"
#include "BezierCurve.h"

BezierSurface::BezierSurface(
    int u_mord, int v_mord, ControlPoint* cp,
    int u_cp_size, int v_cp_size, GLdouble* color, GLdouble resol)
{
    _ordU = u_mord; _ordV = v_mord;
    _ncpntU = u_cp_size; _ncpntV = v_cp_size;
    _min_draw_param_U = 0.0;  _max_draw_param_U = 1.0;
    _min_draw_param_V = 0.0;  _max_draw_param_V = 1.0;

    SetControlPoint(cp, u_cp_size * v_cp_size);
    SetColor(color);
    _resolution = resol;

    // VBO���g��
    _isUseVBO = true;
}

// �w�肵���[�̋Ȑ����擾����
Curve* BezierSurface::GetEdgeCurve(SurfaceEdge edge)
{
    vector<ControlPoint> edge_cp = GetEdgeCurveControlPoint(edge);
    int edge_ord = (edge == U_min || edge == U_max) ? _ordU : _ordV;

    return new BezierCurve(edge_ord, &edge_cp[0], (int)edge_cp.size(), _color, _mesh_width);
}

// ���O�`��
void BezierSurface::PreDraw()
{
    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;

    pnt.resize(101);
    for (int i = 0; i < 101; i++)
        pnt[i].resize(101);

    nor.resize(101);
    for (int i = 0; i < 101; i++)
        nor[i].resize(101);

    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt[i][j] = GetPositionVector(u, v);
            nor[i][j] = GetNormalVector(u, v).Normalize();
        }
    }

    //glColor4dv(_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _color);

    // �O�p�|���S���\��
    for (int i = 0; i <= 99; i += 1)
    {
        for (int j = 0; j <= 99; j += 1)
        {
            glBegin(GL_TRIANGLE_STRIP);

            glNormal3d(nor[i][j]);
            glVertex3d(pnt[i][j]);

            glNormal3d(nor[i + 1][j]);
            glVertex3d(pnt[i + 1][j]);

            glNormal3d(nor[i][j + 1]);
            glVertex3d(pnt[i][j + 1]);
            
            glNormal3d(nor[i + 1][j + 1]);
            glVertex3d(pnt[i + 1][j + 1]);
            
            glEnd();
        }
    }
}

// ���b�V���`��
void BezierSurface::DrawMeshInternal()
{
    Vector3d pnt;

    // ���b�V���̐F��3dv�œn���������Y��(��=0)
    glColor3dv(_color);
    glLineWidth(_mesh_width);

    // U����
    for (int i = 0; i <= 100; i += 5)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = 0; j <= 100; j += 5)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(u, v);
            glVertex3d(pnt);
        }

        glEnd();
    }
    // V����
    for (int i = 0; i <= 100; i += 5)
    {
        glBegin(GL_LINE_STRIP);

        for (int j = 0; j <= 100; j += 5)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt = GetPositionVector(v, u);
            glVertex3d(pnt);
        }

        glEnd();
    }
}

// ���_�o�b�t�@�쐬
void BezierSurface::CreateVBO()
{
    vector<vector<Vector3d>> pnt;
    vector<vector<Vector3d>> nor;
    vector<Vector3d> pnt_vbo;
    vector<Vector3d> nor_vbo;

    pnt.resize(101);
    for (int i = 0; i < 101; i++)
        pnt[i].resize(101);

    nor.resize(101);
    for (int i = 0; i < 101; i++)
        nor[i].resize(101);

    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt[i][j] = GetPositionVector(u, v);
            nor[i][j] = GetNormalVector(u, v).Normalize();
        }
    }

    // VBO�p�̒��_�擾
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            pnt_vbo.push_back(pnt[i][j]);
            pnt_vbo.push_back(pnt[i + 1][j]);
            pnt_vbo.push_back(pnt[i + 1][j + 1]);

            pnt_vbo.push_back(pnt[i][j]);
            pnt_vbo.push_back(pnt[i][j + 1]);
            pnt_vbo.push_back(pnt[i + 1][j + 1]);

            // �@��
            nor_vbo.push_back(nor[i][j]);
            nor_vbo.push_back(nor[i + 1][j]);
            nor_vbo.push_back(nor[i + 1][j + 1]);

            nor_vbo.push_back(nor[i][j]);
            nor_vbo.push_back(nor[i][j + 1]);
            nor_vbo.push_back(nor[i + 1][j + 1]);
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
void BezierSurface::DrawVBO()
{
    //glColor4dv(_color);
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

// �ڐ��x�N�g���`��
void BezierSurface::DrawFirstDiffVectorsInternal()
{
    Vector3d pnt, diff;
    glLineWidth(2.0);

    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        for (int j = 0; j <= 100; j += 5)
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
void BezierSurface::DrawSecondDiffVectorsInternal()
{
    Vector3d pnt, diff;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        for (int j = 0; j <= 100; j += 5)
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
void BezierSurface::DrawNormalVectorsInternal()
{
    Vector3d pnt, normal;
    glLineWidth(1.0);

    glBegin(GL_LINES);

    for (int i = 0; i <= 100; i += 5)
    {
        for (int j = 0; j <= 100; j += 5)
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
void BezierSurface::DrawCurvatureVectorsInternal()
{
    Vector3d pnt, curv;
    
    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = 0; i <= 100; i += 10)
    {
        for (int j = 0; j <= 100; j += 10)
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

// �w��p�����[�^�̃x�N�g�������֐�����Z�o����
Vector3d BezierSurface::CalcVector(
    double u, double v,
    function<double(unsigned, unsigned, double)> BasisFuncU,
    function<double(unsigned, unsigned, double)> BasisFuncV)
{
    Vector3d vector;

    // ���֐��z��(�s��v�Z�p)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // ���֐��z��֊e���֐�����
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = BasisFuncU(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = BasisFuncV(i, _ordV - 1, v);

    // �x�N�g���Z�o(�s��v�Z)
    vector = CalcVectorWithBasisFunctions(N_array_U, N_array_V);

    delete[] N_array_U, N_array_V;
    return vector;
}

// �ʒu�x�N�g���擾
Vector3d BezierSurface::GetPositionVector(double u, double v)
{
    // u:0-diff v:0-diff
    return CalcVector(u, v, CalcBernsteinFunc, CalcBernsteinFunc);
}

// �ڐ��x�N�g���擾
Vector3d BezierSurface::GetFirstDiffVectorU(double u, double v)
{
    // u:1-diff v:0-diff
    return CalcVector(u, v, Calc1DiffBernsteinFunc, CalcBernsteinFunc);
}
Vector3d BezierSurface::GetFirstDiffVectorV(double u, double v)
{
    // u:0-diff v:1-diff
    return CalcVector(u, v, CalcBernsteinFunc, Calc1DiffBernsteinFunc);
}

// 2�K�����x�N�g���擾
Vector3d BezierSurface::GetSecondDiffVectorUU(double u, double v)
{
    // u:2diff v:0-diff
    return CalcVector(u, v, Calc2DiffBernsteinFunc, CalcBernsteinFunc);
}
Vector3d BezierSurface::GetSecondDiffVectorUV(double u, double v)
{
    // u:1-diff v:1-diff
    return CalcVector(u, v, Calc1DiffBernsteinFunc, Calc1DiffBernsteinFunc);
}
Vector3d BezierSurface::GetSecondDiffVectorVV(double u, double v)
{
    // u:0-diff v:2-diff
    return CalcVector(u, v, CalcBernsteinFunc, Calc2DiffBernsteinFunc);
}
