#include "BsplineCurve.h"
#include "ControlPoint.h"
#include <iterator>

BsplineCurve::BsplineCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width, double resol)
{
    _ord = mord;
    _ncpnt = cp_size;
    _nknot = mord + cp_size;

    _min_draw_param = knot[_ord - 1];
    _max_draw_param = knot[_ncpnt];

    SetControlPoint(cp, cp_size);
    SetKnotVector(knot, _nknot);
    SetColor(color);
    _width = width;
    _resolution = resol;

    // VBO�g��
    _isUseVBO = true;
}

// �m�b�g�x�N�g���ݒ�
void BsplineCurve::SetKnotVector(double* knot, int size)
{
    if (size <= 0)
        Error::ShowAndExit("�m�b�g�x�N�g���ݒ莸�s", "knot-vector size must be over 0.");

    _knot.reserve(size);
    for (int i = 0; i < size; i++)
        _knot.emplace_back(knot[i]);
}

// �m�b�g�x�N�g�������Ƃɂ��ē_�Q���擾����
// splitSegCnt: �Z�O�����g�����������邩�̉�(�f�t�H���g��1 = �������Ȃ�)
vector<Vector3d> BsplineCurve::GetPointsByKnots(int splitSegCnt)
{
    vector<Vector3d> pnts;
    double skip = (_knot[_ord] - _knot[0]) / (double)splitSegCnt;

    for (int i = 0; i < _knot.size(); ++i)
    {
        // �m�b�g�̊K���[�̔�`�敔��or�d���͏Ȃ�
        if ((i > 0 && i < _ord) || (i >= _knot.size() - _ord && i < _knot.size() - 1))
            continue;

        pnts.push_back(this->GetPositionVector(_knot[i]));

        // �Ō�͒ǉ����Ȃ�
        if (i != _knot.size() - 1)
        {
            for (int j = 1; j < splitSegCnt; j++)
                pnts.push_back(this->GetPositionVector(_knot[i] + skip * j));
        }
    }

    return pnts;
}

// ���O�`��
void BsplineCurve::PreDraw()
{
    Vector3d pnt;

    glColor3dv(_color);
    glLineWidth(_width);
    glBegin(GL_LINE_STRIP);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        glVertex3d(pnt);
    }

    glEnd();
}

// ���_�o�b�t�@�쐬
void BsplineCurve::CreateVBO()
{
    vector<Vector3d> pnts;

    // ���_�擾
    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;
        pnts.push_back(GetPositionVector(t));
    }

    _nVertex = (int)pnts.size();

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, pnts.size() * 3 * sizeof(double), (GLdouble*)&pnts[0], GL_DYNAMIC_DRAW);
}

// VBO�ŕ`��
void BsplineCurve::DrawVBO()
{
    glColor3dv(_color);
    glLineWidth(_width);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINE_STRIP, 0, _nVertex);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// �ڐ��x�N�g���`��
void BsplineCurve::DrawFirstDiffVectorsInternal()
{
    Vector3d pnt, diff;

    glColor3dv(Color::red);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetFirstDiffVector(t).Normalize();
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// 2�K�����x�N�g���`��
void BsplineCurve::DrawSecondDiffVectorsInternal()
{
    Vector3d pnt, diff;

    glColor3dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetSecondDiffVector(t).Normalize();
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// �@���x�N�g���`��
void BsplineCurve::DrawNormalVectorsInternal()
{
    Vector3d pnt, normal;

    glColor3dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        normal = GetNormalVector(t).Normalize();
        glVertex3d(pnt);
        glVertex3d(pnt + normal);
    }

    glEnd();
}

// �ȗ����a�`��
void BsplineCurve::DrawCurvatureVectorsInternal()
{
    Vector3d pnt, curv;

    glLineWidth(1.0);
    glPointSize(5.0);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        curv = GetCurvatureVector(t);

        // �ȗ����a�`��
        glColor3dv(Color::pink);
        glBegin(GL_LINES);
        glVertex3d(pnt);
        glVertex3d(pnt + curv);
        glEnd();

        // �ȗ����S�`��
        glColor3dv(Color::light_green);
        glBegin(GL_POINTS);
        glVertex3d(pnt + curv);
        glEnd();
    }

    glEnd();
}

// �ʒu�x�N�g���擾
Vector3d BsplineCurve::GetPositionVector(const double t)
{
    Vector3d pnt;

    // �W���I
    for (int i = 0; i < _ncpnt; i++)
        pnt += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return pnt;
}

// �ڐ��x�N�g���擾
Vector3d BsplineCurve::GetFirstDiffVector(double t)
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// 2�K�����x�N�g���擾
Vector3d BsplineCurve::GetSecondDiffVector(double t)
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc2DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// �ʉߓ_����t�ϊ����ċȐ����擾
// TODO: ���Ԃ��������琮������
Curve* BsplineCurve::GetCurveFromPoints(vector<Vector3d> pnts, GLdouble* color, GLdouble width)
{
    // TODO: �Ȑ��O���琶��

    int passPntsCnt = (int)pnts.size(); // �ʉߓ_��
    // _ord = ord;    // �K���͂��Ƃ̋Ȑ��Ɠ����ɂ���
    int new_ncpnt = (passPntsCnt - 1) + (_ord - 1); // �V��������_��
    int new_nknot = _ord + new_ncpnt; // �V�����m�b�g�x�N�g���̑傫��

    // ---- �m�b�g�x�N�g���̌��� ----
    
    vector<double> new_knots;
    new_knots.resize(new_nknot);

    vector<double> seg_dist; // �Z�O�����g�Ԃ̋���
    seg_dist.resize(passPntsCnt - 1);

    double minParam = this->GetMinDrawParam();
    double maxParam = this->GetMaxDrawParam();
    double paramRange = fabs(maxParam - minParam);

    // �ʉߓ_�Ԃ̋����̑��a
    double sum = 0.0;
    for (int i = 0; i < passPntsCnt - 1; ++i)
    {
        seg_dist[i] = pnts[i].DistanceFrom(pnts[i + 1]);
        sum += seg_dist[i];
    }

    // �m�b�g�Ԋu�̊����͒ʉߓ_�Ԃ̋����ɔ�Ⴓ����
    for (int i = 0; i < new_nknot; i++)
    {
        if (i < _ord) // �ŏ��͊K�����d�˂�
            new_knots[i] = minParam;
        else if (i < new_nknot - _ord) // �����ɔ��
        {
            new_knots[i] = new_knots[i - 1] + (seg_dist[i - _ord] / sum) * paramRange;
        }
        else // �Ō���K�����d�˂�
            new_knots[i] = maxParam;
    }

    // ---------------------------------

    // ---- �V��������_�����߂邽�߂̘A���������̏��� ----

    // 1. �A���������������p�̒ʉߓ_�x�N�g��
        vector<Vector3d> P_array;

        P_array.push_back(pnts[0]);
        P_array.push_back(Vector3d::Zero());
        for (int i = 1; i < passPntsCnt - 1; ++i)
            P_array.push_back(pnts[i]);
        P_array.push_back(Vector3d::Zero());
        P_array.push_back(pnts[passPntsCnt - 1]);

        // ���ꂼ��̗v�f���ˉe(select��C++�ł̂����������v�����Ȃ�)
        vector<double> P_array_x, P_array_y, P_array_z;

        for (auto it = P_array.begin(); it != P_array.end(); ++it)
        {
            P_array_x.push_back((*it).X);
            P_array_y.push_back((*it).Y);
            P_array_z.push_back((*it).Z);
        }

    // 2. ���֐��p�s��
        // �s��
        double **N_matrix = new double*[new_ncpnt];
        for (int i = 0; i < new_ncpnt; i++)
            N_matrix[i] = new double[new_ncpnt];

        // ��s��ɏ�����
        for (int i = 0; i < new_ncpnt; i++)
        {
            for (int j = 0; j < new_ncpnt; j++)
                N_matrix[i][j] = 0;
        }

        // ���֐��s����쐬
        for (int i = 0; i < new_ncpnt; i++)
        {
            if (i == 0) // �I�[���� �O
            {
                // ��ԍ��̂�1
                N_matrix[i][0] = 1;
            }
            else if (i == 1) // 2�K�����I�[���� �O
            {
                for (int j = i - 1; j < _ord - 1; ++j)
                    N_matrix[i][j] = Calc2DiffBsplineFunc(j, _ord, new_knots[_ord - 1], &new_knots[0]);
            }
            else if (i > 1 && i < new_ncpnt - 2)
            {
                for (int j = i - 1; j < (_ord - 1) + (i - 1); ++j)
                    N_matrix[i][j] = CalcBsplineFunc(j, _ord, new_knots[(_ord - 1) + (i - 1)], &new_knots[0]);
            }
            else if (i == new_ncpnt - 2) // 2�K�����I�[���� ��
            {
                for (int j = i - 1; j < (_ord - 1) + (i - 1); ++j)
                    N_matrix[i][j] = Calc2DiffBsplineFunc(j, _ord, new_knots[(_ord - 1) + (i - 1)], &new_knots[0]);
            }
            else // �I�[���� ��
            {
                // ��ԉE�̂�1
                N_matrix[i][new_ncpnt - 1] = 1;
            }
        }

        // �����ɍ��킹��1������
        vector<double> N_matrix_flat;
        N_matrix_flat.resize(new_ncpnt * new_ncpnt);

        for (int i = 0; i < new_ncpnt; i++)
        {
            for (int j = 0; j < new_ncpnt; j++)
                N_matrix_flat[i * new_ncpnt + j] = N_matrix[i][j];
        }

    // ------------------------------------------------

    // ----- �V��������_�����߂� -----

    vector<double> N_matrix_flat_x, N_matrix_flat_y, N_matrix_flat_z;
    copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_x));
    copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_y));
    copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_z));

    vector<ControlPoint> new_cps;
    vector<double> new_cps_X, new_cps_Y, new_cps_Z;

    // �A��������������
    new_cps_X = LUDecomposition(new_ncpnt, &N_matrix_flat_x[0], &P_array_x[0]);
    new_cps_Y = LUDecomposition(new_ncpnt, &N_matrix_flat_y[0], &P_array_y[0]);
    new_cps_Z = LUDecomposition(new_ncpnt, &N_matrix_flat_z[0], &P_array_z[0]);

    for (int i = 0; i < new_ncpnt; ++i)
        new_cps.push_back(ControlPoint(new_cps_X[i], new_cps_Y[i], new_cps_Z[i]));

    // ---------------------------------

    return new BsplineCurve(_ord, &new_cps[0], new_ncpnt, &new_knots[0], color, width);
}