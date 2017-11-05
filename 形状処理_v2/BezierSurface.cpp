#include "BezierSurface.h"
#include "BezierCurve.h"

BezierSurface::BezierSurface(
    int u_mord, int v_mord, ControlPoint* cp,
    int u_cp_size, int v_cp_size, GLdouble* color, GLdouble width)
{
    _ordU = u_mord; _ordV = v_mord;
    _ncpntU = u_cp_size; _ncpntV = v_cp_size;
    _min_draw_param_U = 0.0;  _max_draw_param_U = 1.0;
    _min_draw_param_V = 0.0;  _max_draw_param_V = 1.0;

    SetControlPoint(cp, u_cp_size * v_cp_size);
    SetColor(color);
    _mesh_width = width;

    // VBOðg¤
    _isUseVBO = true;
}

// O`æ
void BezierSurface::PreDraw()
{
    vector<vector<Vector3d>> pnt;

    pnt.resize(101);
    for (int i = 0; i < 101; i++)
        pnt[i].resize(101);

    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt[i][j] = GetPositionVector(u, v);
        }
    }

    glColor4dv(_color);

    // Op|S\¦
    for (int i = 0; i <= 99; i += 1)
    {
        for (int j = 0; j <= 99; j += 1)
        {
            glBegin(GL_TRIANGLE_STRIP);
            glVertex3d(pnt[i][j]);
            glVertex3d(pnt[i + 1][j]);
            glVertex3d(pnt[i][j + 1]);
            glVertex3d(pnt[i + 1][j + 1]);
            glEnd();
        }
    }
}

// bV`æ
void BezierSurface::DrawMeshInternal()
{
    Vector3d pnt;

    // bVÌFÍ3dvÅnµ½ûªãYí(¿=0)
    glColor3dv(_color);
    glLineWidth(_mesh_width);

    // Uûü
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
    // Vûü
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

// ¸_obt@ì¬
void BezierSurface::CreateVBO()
{
    vector<vector<Vector3d>> pnt;
    vector<Vector3d> pnt_vbo;

    pnt.resize(101);
    for (int i = 0; i < 101; i++)
        pnt[i].resize(101);

    for (int i = 0; i < 101; i++)
    {
        for (int j = 0; j < 101; j++)
        {
            double u = (double)i / 100;
            double v = (double)j / 100;

            pnt[i][j] = GetPositionVector(u, v);
        }
    }

    // VBOpÌ¸_æ¾
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
        }
    }

    _nVertex = (int)pnt_vbo.size();

    // VBOÌÝè
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// VBOÅ`æ
void BezierSurface::DrawVBO()
{
    glColor4dv(_color);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, _nVertex);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ÚüxNg`æ
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

            // Uûü
            glColor3dv(Color::red); // Ô
            pnt = GetPositionVector(u, v);
            diff = GetFirstDiffVectorU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // Vûü
            glColor3dv(Color::green); // Î
            diff = GetFirstDiffVectorV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// 2K÷ªxNg`æ
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

            // UU÷ª
            glColor3dv(Color::blue); // Â
            pnt = GetPositionVector(u, v);
            diff = GetSecondDiffVectorUU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // UV÷ª
            glColor3dv(Color::blue); // Â
            diff = GetSecondDiffVectorUV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // VV÷ª
            glColor3dv(Color::blue); // Â
            diff = GetSecondDiffVectorVV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// @üxNg`æ
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

            // @ü
            glColor3dv(Color::blue); // Â
            pnt = GetPositionVector(u, v);
            normal = GetNormalVector(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + normal);
        }
    }

    glEnd();
}

// È¦¼a`æ
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

            // È¦¼a
            glColor3dv(Color::pink); // sN
            glBegin(GL_LINES);
            glVertex3d(pnt);
            glVertex3d(pnt + curv);
            glEnd();

            // È¦S
            glColor3dv(Color::light_green); // ©Î
            glBegin(GL_POINTS);
            glVertex3d(pnt + curv);
            glEnd();
        }
    }
}

// ÊuxNgæ¾
Vector3d BezierSurface::GetPositionVector(double u, double v)
{
    Vector3d pnt;
    double temp[100]; // vZp

    // îêÖzñ(sñvZp)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // îêÖzñÖeîêÖðãü
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBernsteinFunc(i, _ordV - 1, v);

    // ÊuxNgZo(sñvZ)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    pnt.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    pnt.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    pnt.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return pnt;
}

// ÚüxNgæ¾
Vector3d BezierSurface::GetFirstDiffVectorU(double u, double v)
{
    Vector3d diff;
    double temp[100]; // vZp

    // îêÖzñ(sñvZp)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // îêÖzñÖeîêÖðãü
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc1DiffBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBernsteinFunc(i, _ordV - 1, v);

    // ÊuxNgZo(sñvZ)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BezierSurface::GetFirstDiffVectorV(double u, double v)
{
    Vector3d diff;
    double temp[100]; // vZp

    // îêÖzñ(sñvZp)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // îêÖzñÖeîêÖðãü
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc1DiffBernsteinFunc(i, _ordV - 1, v);

    // ÊuxNgZo(sñvZ)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}

// 2K÷ªxNgæ¾
Vector3d BezierSurface::GetSecondDiffVectorUU(double u, double v)
{
    Vector3d diff;
    double temp[100]; // vZp

    // îêÖzñ(sñvZp)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // îêÖzñÖeîêÖðãü
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc2DiffBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = CalcBernsteinFunc(i, _ordV - 1, v);

    // ÊuxNgZo(sñvZ)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BezierSurface::GetSecondDiffVectorUV(double u, double v)
{
    Vector3d diff;
    double temp[100]; // vZp

    // îêÖzñ(sñvZp)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // îêÖzñÖeîêÖðãü
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = Calc1DiffBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc1DiffBernsteinFunc(i, _ordV - 1, v);

    // ÊuxNgZo(sñvZ)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}
Vector3d BezierSurface::GetSecondDiffVectorVV(double u, double v)
{
    Vector3d diff;
    double temp[100]; // vZp

    // îêÖzñ(sñvZp)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // îêÖzñÖeîêÖðãü
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = CalcBernsteinFunc(i, _ordU - 1, u);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = Calc2DiffBernsteinFunc(i, _ordV - 1, v);

    // ÊuxNgZo(sñvZ)
    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpX[0], temp);
    diff.X = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpY[0], temp);
    diff.Y = MatrixMultiply(_ncpntV, temp, N_array_V);

    MatrixMultiply(1, _ncpntU, _ncpntV, N_array_U, &_ctrlpZ[0], temp);
    diff.Z = MatrixMultiply(_ncpntV, temp, N_array_V);

    delete[] N_array_U, N_array_V;
    return diff;
}

// wèµ½[ÌÈüðæ¾·é
Curve* BezierSurface::GetEdgeCurve(SurfaceEdge edge)
{
    vector<ControlPoint> edge_cp = GetEdgeCurveControlPoint(edge);
    int edge_ord = (edge == U_min || edge == U_max) ? _ordU : _ordV;

    return new BezierCurve(edge_ord, &edge_cp[0], (int)edge_cp.size(), _color, _mesh_width);
}