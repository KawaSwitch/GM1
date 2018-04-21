#include "BsplineSurface.h"
#include "BsplineCurve.h"
#include <iterator>

BsplineSurface::BsplineSurface(
    const int u_mord, const int v_mord, const ControlPoint* const cp,
    const int u_cp_size, const int v_cp_size, const double* const u_knot, const double* const v_knot,
    const GLdouble* const color, const GLdouble resol)
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
    _resolution = resol;

    // VBO���g��
    _isUseVBO = true;
    // IBO���g��
    //_isUseIBO = true;
}

// �m�b�g�x�N�g���ݒ�
void BsplineSurface::SetKnotVector(const double* const knot, int size, vector<double>& _knot)
{
    if (size <= 0)
        Error::ShowAndExit("�m�b�g�x�N�g���ݒ莸�s", "knot-vector size must be over 0.");

    _knot.reserve(size);
    for (int i = 0; i < size; i++)
        _knot.emplace_back(knot[i]);
}

// �w�肵���[�̋Ȑ����擾����
Curve* BsplineSurface::GetEdgeCurve(const SurfaceEdge edge) const
{
    vector<ControlPoint> edge_cp = GetEdgeCurveControlPoint(edge);
    int edge_ord = (edge == U_min || edge == U_max) ? _ordV : _ordU;
    vector<double> edge_knot = (edge == U_min || edge == U_max) ? _knotV : _knotU;

    return new BsplineCurve(edge_ord, &edge_cp[0], (int)edge_cp.size(), &edge_knot[0], Color::red, _mesh_width);
}

// ���O�`��
void BsplineSurface::PreDraw() const
{   
    // �𑜓x
    int RES = (int)_resolution;

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
void BsplineSurface::DrawMeshInternal() const
{
    Vector3d pnt;

    // ���b�V���̐F��3dv�œn���������Y��(��=0)
    glColor4dv(_color);
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
void BsplineSurface::CreateVBO() const
{
    // �𑜓x
    int RES = (int)_resolution;

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
            // ��O�p
            pnt_vbo.push_back(pnt[i - u_min][j - v_min]);
            pnt_vbo.push_back(pnt[i + 1 - u_min][j - v_min]);
            pnt_vbo.push_back(pnt[i - u_min][j + 1 - v_min]);

            // ���O�p
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

    _nVertex_cache = (int)pnt_vbo.size();

    // VBO�̐ݒ�
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex_cache * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_vbo_nor);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex_cache * 3, &nor_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// VBO�ŕ`��
void BsplineSurface::DrawVBO() const
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
    glDrawArrays(GL_TRIANGLES, 0, _nVertex_cache);

    // clean up
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ���_�o�b�t�@(IBO)�쐬
void BsplineSurface::CreateIBO() const
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

    _nVertex_cache = (int)pnt_vbo.size();

    // VBO�̐ݒ�
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex_cache * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // IBO�̐ݒ�
    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(double) * _nVertex_cache * 3, &pnt_vbo[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glGenBuffers(1, &_vbo_nor);
    //glBindBuffer(GL_ARRAY_BUFFER, _vbo_nor);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(double) * _nVertex * 3, &nor_vbo[0], GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// IBO�ŕ`��
void BsplineSurface::DrawIBO() const
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
    glDrawElements(GL_TRIANGLES, _nVertex_cache, GL_UNSIGNED_INT, (void *)0);

    // clean up
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// �ڐ��x�N�g���`��
void BsplineSurface::DrawFirstDiffVectorsInternal() const
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
            glColor4dv(Color::red); // ��
            pnt = GetPositionVector(u, v);
            diff = GetFirstDiffVectorU(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);

            // V����
            glColor4dv(Color::green); // ��
            diff = GetFirstDiffVectorV(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + diff);
        }
    }

    glEnd();
}

// 2�K�����x�N�g���`��
void BsplineSurface::DrawSecondDiffVectorsInternal() const
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
void BsplineSurface::DrawNormalVectorsInternal() const
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
            glColor4dv(Color::blue); // ��
            pnt = GetPositionVector(u, v);
            normal = GetNormalVector(u, v).Normalize();
            glVertex3d(pnt);
            glVertex3d(pnt + normal);
        }
    }

    glEnd();
}

// �ȗ����a�`��
void BsplineSurface::DrawCurvatureVectorsInternal() const
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
            glColor4dv(Color::pink); // �s���N
            glBegin(GL_LINES);
            glVertex3d(pnt);
            glVertex3d(pnt + curv);
            glEnd();

            // �ȗ����S
            glColor4dv(Color::light_green); // ����
            glBegin(GL_POINTS);
            glVertex3d(pnt + curv);
            glEnd();
        }
    }
}

// �w��p�����[�^�̃x�N�g�������֐�����Z�o����
Vector3d BsplineSurface::CalcVector(
    const double u, const double v,
    const function<double(const unsigned, const unsigned, const double, const double* const)> BasisFuncU,
    const function<double(const unsigned, const unsigned, const double, const double* const)> BasisFuncV) const
{
    Vector3d vector;

    // ���֐��z��(�s��v�Z�p)
    double* N_array_U = new double[_ncpntU];
    double* N_array_V = new double[_ncpntV];

    // ���֐��z��֊e���֐�����
    for (int i = 0; i < _ncpntU; i++)
        N_array_U[i] = BasisFuncU(i, _ordU, u, &_knotU[0]);
    for (int i = 0; i < _ncpntV; i++)
        N_array_V[i] = BasisFuncV(i, _ordV, v, &_knotV[0]);

    // �x�N�g���Z�o(�s��v�Z)
    vector = CalcVectorWithBasisFunctions(N_array_U, N_array_V);

    delete[] N_array_U, N_array_V;
    return vector;
}

// �ʒu�x�N�g���擾
Vector3d BsplineSurface::GetPositionVector(const double u, const double v) const
{
    // u:0-diff v:0-diff
    return CalcVector(u, v, CalcBsplineFunc, CalcBsplineFunc);
}

// �ڐ��x�N�g���擾
Vector3d BsplineSurface::GetFirstDiffVectorU(const double u, const double v) const
{
    // u:1-diff v:0-diff
    return CalcVector(u, v, Calc1DiffBsplineFunc, CalcBsplineFunc);
}
Vector3d BsplineSurface::GetFirstDiffVectorV(const double u, const double v) const
{
    // u:0-diff v:1-diff
    return CalcVector(u, v, CalcBsplineFunc, Calc1DiffBsplineFunc);
}

// 2�K�����x�N�g���擾
Vector3d BsplineSurface::GetSecondDiffVectorUU(const double u, const double v) const
{
    // u:2diff v:0-diff
    return CalcVector(u, v, Calc2DiffBsplineFunc, CalcBsplineFunc);
}
Vector3d BsplineSurface::GetSecondDiffVectorUV(const double u, const double v) const
{
    // u:1-diff v:1-diff
    return CalcVector(u, v, Calc1DiffBsplineFunc, Calc1DiffBsplineFunc);
}
Vector3d BsplineSurface::GetSecondDiffVectorVV(const double u, const double v) const
{
    // u:0-diff v:2-diff
    return CalcVector(u, v, CalcBsplineFunc, Calc2DiffBsplineFunc);
}

// �m�b�g�x�N�g�������Ƃɂ��ē_�Q���擾����
// splitSegCnt: �Z�O�����g�����������邩�̉�(�f�t�H���g��1 = �������Ȃ�)
vector<vector<Vector3d>> BsplineSurface::GetPointsByKnots(const int splitSegCnt_U, const int splitSegCnt_V) const
{
    vector<vector<Vector3d>> pnts; // pnts[v][u]�ƕ��ׂ�

    double skip;
    vector<double> knotsU, knotsV; // �_�Q���擾����p�����[�^

    // U�����p�����[�^�ʒu
    {
        for (size_t i = _ordU - 1, kn = _knotU.size(); i < kn - _ordU; ++i)
        {
            skip = (_knotU[i + 1] - _knotU[i]) / (double)splitSegCnt_U;
            
            for (int j = 0; j < splitSegCnt_U; ++j)
                knotsU.push_back(_knotU[i] + skip * j);
        }
        knotsU.push_back(_knotU[_knotU.size() - _ordU]);
    }
    // V�����p�����[�^�ʒu
    {
        for (size_t i = _ordV - 1, kn = _knotV.size(); i < kn - _ordV; ++i)
        {
            skip = (_knotV[i + 1] - _knotV[i]) / (double)splitSegCnt_V;

            for (int j = 0; j < splitSegCnt_V; ++j)
                knotsV.push_back(_knotV[i] + skip * j);
        }
        knotsV.push_back(_knotV[_knotV.size() - _ordV]);
    }

    // �ʒu�x�N�g�������߂�
    for (size_t j = 0, kv_size = knotsV.size(); j < kv_size; ++j)
    {
        vector<Vector3d> p_vec;
        p_vec.reserve(knotsU.size());

        for (size_t i = 0, ku_size = knotsU.size(); i < ku_size; ++i)
            p_vec.emplace_back(GetPositionVector(knotsU[i], knotsV[j]));

        pnts.push_back(p_vec);
    }

    return pnts;
}

// �ʉߓ_���������Z�b�g����Ă��邩
// U����/V�����̓_�̐������ꂼ�ꓙ���������`�F�b�N����
bool IsPassingPntsSetCorrectly(const vector<vector<Vector3d>>& pnts)
{
    if (pnts.size() < 1)
        return false;

    // ����(u����)�������ׂ�΂悢
    for (const auto& u_pnts : pnts)
    {
        if (pnts[0].size() != u_pnts.size())
            return false;
    }

    // ���ׂē������Ȃ琳����(n�~m��)
    return true;
}

// �ʉߓ_�ƊK������m�b�g�x�N�g�����Z�o����
void CalcKnotVectorByPassingPnts(const vector<Vector3d>& pnts, const int ord, vector<double>* knot)
{
    if (knot == nullptr)
    {
        Error::ShowAndExit("�k���|�C���^");
        return;
    }

    // �m�b�g��T�C�Y�͐���_���ƊK���̘a
    knot->resize((pnts.size() + ord - 2) + ord);

    vector<double> seg_dist; // �Z�O�����g�Ԃ̋���
    seg_dist.resize(pnts.size() - 1);

    double minParam = 0.0;
    double maxParam = 1.0;
    double paramRange = fabs(maxParam - minParam);

    // �ʉߓ_�Ԃ̋����̑��a
    double sum = 0.0;
    for (size_t i = 0, n = pnts.size(); i < n - 1; ++i)
    {
        seg_dist[i] = pnts[i].DistanceFrom(pnts[i + 1]);
        sum += seg_dist[i];
    }

    // �m�b�g�Ԋu�̊����͒ʉߓ_�Ԃ̋����ɔ�Ⴓ����
    for (size_t i = 0, n = knot->size(); i < n; ++i)
    {
        if (i < ord) // �ŏ��͊K�����d�˂�
            (*knot)[i] = minParam;
        else if (i < knot->size() - ord) // �����ɔ��
        {
            (*knot)[i] = (*knot)[i - 1] + (seg_dist[i - ord] / sum) * paramRange;
        }
        else // �Ō���K�����d�˂�
            (*knot)[i] = maxParam;
    }
}

// �����̃m�b�g��𐳋K������
void NormalizeKnotVector(const vector<vector<double>>& knot_array, const int ord, vector<double>* knot_normal)
{
    if (knot_normal == nullptr)
    {
        Error::ShowAndExit("�k���|�C���^");
        return;
    }
    if (knot_array.size() < 1)
    {
        Error::ShowAndExit("��w��");
        return;
    }

    // ���K���O��0�Ԗڂ̃m�b�g��ŏ��������Ă���
    knot_normal->resize(knot_array[0].size());
    for (size_t i = 0, n = knot_array[0].size(); i < n; ++i)
        (*knot_normal)[i] = knot_array[0][i];

    // �K�v�Ȍ�(�K������)�𐳋K������
    for (size_t col = ord, n = knot_array[0].size(); col < n - ord; ++col)
    {
        double sum = 0.0;

        for (size_t row = 0, n = knot_array.size(); row < n; ++row)
            sum += knot_array[row][col];

        // ����
        (*knot_normal)[col] = sum / knot_array.size();
    }
}

// �m�b�g�x�N�g�����擾����
void CalcKnotVectorByPassingPnts(const vector<vector<Vector3d>>& pnts, const int ordU, const int ordV,
    vector<double>* knotU, vector<double>* knotV)
{
    // �e�����̒ʉߓ_��
    int passPntsCntU, passPntsCntV;
    passPntsCntU = (int)pnts[0].size();
    passPntsCntV = (int)pnts.size();

    // �e�����̒ʉߓ_����擾����m�b�g��Q
    vector<vector<double>> new_knotsU(passPntsCntV), new_knotsV(passPntsCntU);

    // U�����m�b�g�x�N�g���̌���
    {
        for (int i = 0; i < passPntsCntV; ++i)
        {
            vector<Vector3d> pntsU(passPntsCntU);
            vector<double> knot;

            for (int j = 0; j < passPntsCntU; ++j)
                pntsU[j] = pnts[i][j];

            CalcKnotVectorByPassingPnts(pntsU, ordU, &knot);
            new_knotsU[i] = knot;
        }
        // ���K��
        NormalizeKnotVector(new_knotsU, ordU, knotU);
    }
    // V�����m�b�g�x�N�g���̌���
    {
        for (int i = 0; i < passPntsCntU; ++i)
        {
            vector<Vector3d> pntsV(passPntsCntV);
            vector<double> knot;

            for (int j = 0; j < passPntsCntV; ++j)
                pntsV[j] = pnts[j][i];

            CalcKnotVectorByPassingPnts(pntsV, ordV, &knot);
            new_knotsV[i] = knot;
        }
        // ���K��
        NormalizeKnotVector(new_knotsV, ordV, knotV);
    }
}

// 1�����̓_�Q���炻����ʂ�Ȑ��̐���_���Z�o����
/*
    �ʉߓ_�����ƏI�[��������A�����������쐬������
    ������, �ʉߓ_��n ����_��qn(n + �K�� - 2) �m�b�g��T�C�Ykn �Ƃ���

    [�ʉߓ_����]
        <�[�_����>
            P(k_0) = Q_0
            P(k_kn-1) = Q_qn-1
        <����ȊO�̒ʉߓ_����>
            P(k_�K��) = Q_1
            P(k_�K��+1) = Q_2
            ...
            P(k_kn-�K��-1) = Q_qn-2

    [�I�[����]
        P''(k_�K��-1) = 0
        P''(k_kn-�K��-2) = 0
*/
void CalcControlPointsByPassingPnts(const vector<Vector3d>& pnts, const int ord, const vector<double>& knot, vector<ControlPoint>* ctrlp)
{
    // �e�����̒ʉߓ_��
    int passPntsCnt = (int)pnts.size();
    // �V��������_��
    int new_ncpnt = (passPntsCnt - 1) + (ord - 1);

    // �ʉߓ_�x�N�g��(�A���������p)
    vector<Vector3d> P_array;
    vector<double> P_array_x, P_array_y, P_array_z; // �ˉe��

    // ���֐��s��
    double* N_matrix;

    // 1. �A���������������p�̒ʉߓ_�x�N�g�� �쐬
    {
        P_array.push_back(pnts[0]);
        P_array.push_back(Vector3d::Zero());
        for (int i = 1; i < passPntsCnt - 1; ++i)
            P_array.push_back(pnts[i]);
        P_array.push_back(Vector3d::Zero());
        P_array.push_back(pnts[passPntsCnt - 1]);

        // �v�Z�p�Ɋe�v�f�Ɏˉe
        for (auto it = P_array.begin(), end = P_array.end(); it != end; ++it)
        {
            P_array_x.push_back((*it).X);
            P_array_y.push_back((*it).Y);
            P_array_z.push_back((*it).Z);
        }
    }

    // 2. ���֐��p�s�� �쐬
    {
        N_matrix = new double[new_ncpnt * new_ncpnt];

        // ��s��ɏ�����
        for (int i = 0; i < new_ncpnt; i++)
        {
            for (int j = 0; j < new_ncpnt; j++)
                N_matrix[i * new_ncpnt + j] = 0;
        }

        for (int i = 0; i < new_ncpnt; i++)
        {
            if (i == 0) // �I�[���� �O
            {
                // ��ԍ��̂�1
                N_matrix[i * new_ncpnt + 0] = 1;
            }
            else if (i == 1) // 2�K�����I�[���� �O
            {
                for (int j = i - 1; j < ord - 1; ++j)
                    N_matrix[i * new_ncpnt + j] = Calc2DiffBsplineFunc(j, ord, knot[ord - 1], &(const_cast<vector<double>&>(knot))[0]);
            }
            else if (i > 1 && i < new_ncpnt - 2)
            {
                for (int j = i - 1; j < (ord - 1) + (i - 1); ++j)
                    N_matrix[i * new_ncpnt + j] = CalcBsplineFunc(j, ord, knot[(ord - 1) + (i - 1)], &(const_cast<vector<double>&>(knot))[0]);
            }
            else if (i == new_ncpnt - 2) // 2�K�����I�[���� ��
            {
                for (int j = i - 1; j < (ord - 1) + (i - 1); ++j)
                    N_matrix[i * new_ncpnt + j] = Calc2DiffBsplineFunc(j, ord, knot[(ord - 1) + (i - 1)], &(const_cast<vector<double>&>(knot))[0]);
            }
            else // �I�[���� ��
            {
                // ��ԉE�̂�1
                N_matrix[i * new_ncpnt + (new_ncpnt - 1)] = 1;
            }
        }
    }

    // 3. �V��������_�����߂�
    {
        ctrlp->resize(new_ncpnt);
        vector<double> new_cps_X, new_cps_Y, new_cps_Z;

        // �A��������������
        new_cps_X = LUDecomposition(new_ncpnt, N_matrix, &P_array_x[0]);
        new_cps_Y = LUDecomposition(new_ncpnt, N_matrix, &P_array_y[0]);
        new_cps_Z = LUDecomposition(new_ncpnt, N_matrix, &P_array_z[0]);

        for (int i = 0; i < new_ncpnt; ++i)
            (*ctrlp)[i] = ControlPoint(new_cps_X[i], new_cps_Y[i], new_cps_Z[i]);
    }

    // ���
    delete N_matrix;
}

// �ʉߓ_����t�ϊ����ċȖʂ��擾
// TODO: static��
Surface* BsplineSurface::GetSurfaceFromPoints(const vector<vector<Vector3d>>& pnts, const GLdouble* const color, const GLdouble resol) const
{
    int passPntsCntU, passPntsCntV; // �e�����̒ʉߓ_��
    int ordU, ordV; // �K��
    int new_ncpntU, new_ncpntV; // ����_��
    int new_nknotU, new_nknotV; // �m�b�g��T�C�Y

    vector<double> knotU, knotV; // �m�b�g�x�N�g��
    vector<ControlPoint> cps; // ����_

    // �e�ϐ�������
    if (IsPassingPntsSetCorrectly(pnts))
    {
        passPntsCntU = (int)pnts[0].size();
        passPntsCntV = (int)pnts.size();

        ordU = 4; // �Ƃ肠�����V�X�e���Œ�
        ordV = 4;

        new_ncpntU = (passPntsCntU - 1) + (ordU - 1);
        new_ncpntV = (passPntsCntV - 1) + (ordV - 1);

        new_nknotU = ordU + new_ncpntU;
        new_nknotV = ordV + new_ncpntV;
    }
    else
        return nullptr;

    // �m�b�g�x�N�g���̌���
    CalcKnotVectorByPassingPnts(pnts, ordU, ordV, &knotU, &knotV);

    // ����_�̌���
    {
        vector<vector<ControlPoint>> temp_cpsU(passPntsCntV), temp_cpsV(new_ncpntU);
        cps.resize(new_ncpntU * new_ncpntV);

        // U�������琧��_�Q���Z�o
        for (int i = 0; i < passPntsCntV; ++i)
        {
            vector<Vector3d> pntsU(passPntsCntU);
            vector<ControlPoint> ctrlpU;

            for (int j = 0; j < passPntsCntU; ++j)
                pntsU[j] = pnts[i][j];

            CalcControlPointsByPassingPnts(pntsU, ordU, knotU, &ctrlpU);
            temp_cpsU[i] = ctrlpU;
        }

        // V�������琧��_�Q���Z�o
        for (size_t i = 0; i < new_ncpntU; ++i)
        {
            vector<Vector3d> pntsV(passPntsCntV);
            vector<ControlPoint> ctrlpV;

            for (int j = 0; j < passPntsCntV; ++j)
                pntsV[j] = temp_cpsU[j][i];

            // ��) �ʉߓ_��U��������̐���_
            // �����ŋ��߂�����_���Ȗʂ̐���_�ƂȂ�
            CalcControlPointsByPassingPnts(pntsV, ordV, knotV, &ctrlpV);
            temp_cpsV[i] = ctrlpV;
        }

        // �]�u
        for (int i = 0; i < new_ncpntU; ++i)
        {
            for (int j = 0; j < new_ncpntV; ++j)
            {
                cps[j * new_ncpntU + i] = temp_cpsV[i][j];
            }
        }
    }

    return new BsplineSurface(ordU, ordV, &cps[0], new_ncpntU, new_ncpntV, &knotU[0], &knotV[0], color);
}