#include "BsplineCurve.h"
#include "ControlPoint.h"
#include <iterator>

BsplineCurve::BsplineCurve(const int mord, const ControlPoint* const cp, const int cp_size, const double* const knot,
    const GLdouble* const color, const GLdouble width, const double resol)
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

    _length = this->GetLength();
    _draw_vec_length = _length * _draw_vector_ratio;

    // VBO�g��
    _isUseVBO = true;
}

// �m�b�g�x�N�g���ݒ�
void BsplineCurve::SetKnotVector(const double* const knot, const int size)
{
    if (size <= 0)
        Error::ShowAndExit("�m�b�g�x�N�g���ݒ莸�s", "knot-vector size must be over 0.");

    _knot.reserve(size);
    for (int i = 0; i < size; i++)
        _knot.emplace_back(knot[i]);
}

// �m�b�g�x�N�g�������Ƃɂ��ē_�Q���擾����
// splitSegCnt: �Z�O�����g�����������邩�̉�(�f�t�H���g��1 = �������Ȃ�)
vector<Vector3d> BsplineCurve::GetPositionVectorsByKnots(const int splitSegCnt) const
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
// �m�b�g�x�N�g�������Ƃɂ��ē_�Q���擾����
// splitSegCnt: �Z�O�����g�����������邩�̉�(�f�t�H���g��1 = �������Ȃ�)
vector<Point3dC> BsplineCurve::GetPointsByKnots(const int splitSegCnt) const
{
    vector<Point3dC> pnts;
    double skip = (_knot[_ord] - _knot[0]) / (double)splitSegCnt;

    for (int i = 0; i < _knot.size(); ++i)
    {
        // �m�b�g�̊K���[�̔�`�敔��or�d���͏Ȃ�
        if ((i > 0 && i < _ord) || (i >= _knot.size() - _ord && i < _knot.size() - 1))
            continue;

        pnts.push_back(Point3dC(this->GetPositionVector(_knot[i]), _knot[i]));

        // �Ō�͒ǉ����Ȃ�
        if (i != _knot.size() - 1)
        {
            for (int j = 1; j < splitSegCnt; j++)
                pnts.push_back(Point3dC(this->GetPositionVector(_knot[i] + skip * j), _knot[i] + skip * j));
        }
    }

    return pnts;
}

// ���O�`��
void BsplineCurve::PreDraw() const
{
    Vector3d pnt;

    glColor4dv(_color);
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
void BsplineCurve::CreateVBO() const
{
    vector<Vector3d> pnts;

    // ���_�擾
    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;
        pnts.push_back(GetPositionVector(t));
    }

    _nVertex_cache = (int)pnts.size();

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, pnts.size() * 3 * sizeof(double), (GLdouble*)&pnts[0], GL_DYNAMIC_DRAW);
}

// VBO�ŕ`��
void BsplineCurve::DrawVBO() const
{
    glColor4dv(_color);
    glLineWidth(_width);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINE_STRIP, 0, _nVertex_cache);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// �ڐ��x�N�g���`��
void BsplineCurve::DrawFirstDiffVectorsInternal() const
{
    Vector3d pnt, diff;

    glColor4dv(Color::red);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetFirstDiffVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// 2�K�����x�N�g���`��
void BsplineCurve::DrawSecondDiffVectorsInternal() const
{
    Vector3d pnt, diff;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        diff = GetSecondDiffVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// �@���x�N�g���`��
void BsplineCurve::DrawNormalVectorsInternal() const
{
    Vector3d pnt, normal;

    glColor4dv(Color::blue);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        pnt = GetPositionVector(t);
        normal = GetNormalVector(t).Normalize() * _draw_vec_length;
        glVertex3d(pnt);
        glVertex3d(pnt + normal);
    }

    glEnd();
}

// �ȗ����a�`��
void BsplineCurve::DrawCurvatureVectorsInternal() const
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
        glColor4dv(Color::pink);
        glBegin(GL_LINES);
        glVertex3d(pnt);
        glVertex3d(pnt + curv);
        glEnd();

        // �ȗ����S�`��
        glColor4dv(Color::light_green);
        glBegin(GL_POINTS);
        glVertex3d(pnt + curv);
        glEnd();
    }

    glEnd();
}

// �ʒu�x�N�g���擾
Vector3d BsplineCurve::GetPositionVector(const double t) const
{
    Vector3d pnt;

    // �W���I
    for (int i = 0; i < _ncpnt; i++)
        pnt += CalcBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return pnt;
}

// �ڐ��x�N�g���擾
Vector3d BsplineCurve::GetFirstDiffVector(const double t) const
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}

// 2�K�����x�N�g���擾
Vector3d BsplineCurve::GetSecondDiffVector(const double t) const
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
        diff += Calc2DiffBsplineFunc(i, _ord, t, &_knot[0]) * _ctrlp[i];

    return diff;
}


void CalcControlPointsByPassingPnt(const vector<Vector3d>& pnts, const int ord, const vector<double>& knot, vector<ControlPoint>* ctrlp)
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

// �ʉߓ_����t�ϊ����ċȐ����擾
// TODO: ���Ԃ��������琮������
Curve* BsplineCurve::GetCurveFromPoints(const vector<Vector3d>& pnts, const GLdouble* const color, const GLdouble width) const
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

    vector<ControlPoint> new_cps;
    CalcControlPointsByPassingPnt(pnts, _ord, new_knots, &new_cps);

    // ---------------------------------

    // ---- �V��������_�����߂邽�߂̘A���������̏��� ----

    //// 1. �A���������������p�̒ʉߓ_�x�N�g��
    //    vector<Vector3d> P_array;

    //    P_array.push_back(pnts[0]);
    //    P_array.push_back(Vector3d::Zero());
    //    for (int i = 1; i < passPntsCnt - 1; ++i)
    //        P_array.push_back(pnts[i]);
    //    P_array.push_back(Vector3d::Zero());
    //    P_array.push_back(pnts[passPntsCnt - 1]);

    //    // ���ꂼ��̗v�f���ˉe(select��C++�ł̂����������v�����Ȃ�)
    //    vector<double> P_array_x, P_array_y, P_array_z;

    //    for (auto it = P_array.begin(); it != P_array.end(); ++it)
    //    {
    //        P_array_x.push_back((*it).X);
    //        P_array_y.push_back((*it).Y);
    //        P_array_z.push_back((*it).Z);
    //    }

    //// 2. ���֐��p�s��
    //    // �s��
    //    double **N_matrix = new double*[new_ncpnt];
    //    for (int i = 0; i < new_ncpnt; i++)
    //        N_matrix[i] = new double[new_ncpnt];

    //    // ��s��ɏ�����
    //    for (int i = 0; i < new_ncpnt; i++)
    //    {
    //        for (int j = 0; j < new_ncpnt; j++)
    //            N_matrix[i][j] = 0;
    //    }

    //    // ���֐��s����쐬
    //    for (int i = 0; i < new_ncpnt; i++)
    //    {
    //        if (i == 0) // �I�[���� �O
    //        {
    //            // ��ԍ��̂�1
    //            N_matrix[i][0] = 1;
    //        }
    //        else if (i == 1) // 2�K�����I�[���� �O
    //        {
    //            for (int j = i - 1; j < _ord - 1; ++j)
    //                N_matrix[i][j] = Calc2DiffBsplineFunc(j, _ord, new_knots[_ord - 1], &new_knots[0]);
    //        }
    //        else if (i > 1 && i < new_ncpnt - 2)
    //        {
    //            for (int j = i - 1; j < (_ord - 1) + (i - 1); ++j)
    //                N_matrix[i][j] = CalcBsplineFunc(j, _ord, new_knots[(_ord - 1) + (i - 1)], &new_knots[0]);
    //        }
    //        else if (i == new_ncpnt - 2) // 2�K�����I�[���� ��
    //        {
    //            for (int j = i - 1; j < (_ord - 1) + (i - 1); ++j)
    //                N_matrix[i][j] = Calc2DiffBsplineFunc(j, _ord, new_knots[(_ord - 1) + (i - 1)], &new_knots[0]);
    //        }
    //        else // �I�[���� ��
    //        {
    //            // ��ԉE�̂�1
    //            N_matrix[i][new_ncpnt - 1] = 1;
    //        }
    //    }

    //    // �����ɍ��킹��1������
    //    vector<double> N_matrix_flat;
    //    N_matrix_flat.resize(new_ncpnt * new_ncpnt);

    //    for (int i = 0; i < new_ncpnt; i++)
    //    {
    //        for (int j = 0; j < new_ncpnt; j++)
    //            N_matrix_flat[i * new_ncpnt + j] = N_matrix[i][j];
    //    }

    //// ------------------------------------------------

    //// ----- �V��������_�����߂� -----

    //vector<double> N_matrix_flat_x, N_matrix_flat_y, N_matrix_flat_z;
    //copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_x));
    //copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_y));
    //copy(N_matrix_flat.begin(), N_matrix_flat.end(), back_inserter(N_matrix_flat_z));

    //vector<ControlPoint> new_cps;
    //vector<double> new_cps_X, new_cps_Y, new_cps_Z;

    //// �A��������������
    //new_cps_X = LUDecomposition(new_ncpnt, &N_matrix_flat_x[0], &P_array_x[0]);
    //new_cps_Y = LUDecomposition(new_ncpnt, &N_matrix_flat_y[0], &P_array_y[0]);
    //new_cps_Z = LUDecomposition(new_ncpnt, &N_matrix_flat_z[0], &P_array_z[0]);

    //for (int i = 0; i < new_ncpnt; ++i)
    //    new_cps.push_back(ControlPoint(new_cps_X[i], new_cps_Y[i], new_cps_Z[i]));

    //// ---------------------------------

    return new BsplineCurve(_ord, &new_cps[0], new_ncpnt, &new_knots[0], color, width);
}

// �ŋߓ_�擾
NearestPointInfoC BsplineCurve::GetNearestPointInfoFromRef(const Vector3d& ref) const
{
    const int seg_split = 8; // �Z�O�����g������
    double left, right, middle; // 2���T���p�p�����[�^
    Vector3d pnt, vec_ref_pnt, tan;
    double dot; // ���ϒl

    auto startPnts = this->GetPointsByKnots(seg_split);

    for (unsigned int i = 0, s = startPnts.size(); i < s; ++i)
    {
        int count = 0; // 2���T���X�e�b�v��

        left = startPnts[(i == 0) ? i : i - 1].param;
        middle = startPnts[i].param;
        right = startPnts[(i == s - 1) ? i : i + 1].param;

        pnt = GetPositionVector(middle);
        tan = GetFirstDiffVector(middle);

        vec_ref_pnt = pnt - ref;
        dot = tan.Dot(vec_ref_pnt); // ���ϒl

        while (left <= right)
        {
            // �x�N�g���̓��ς�0
            if (-EPS::NEAREST < dot && dot < EPS::NEAREST)
            {
                // �\���Ȑ��x�Ȃ̂Ō����������Ƃɂ���
                return NearestPointInfoC(pnt, ref, middle);
            }
            else if (dot >= EPS::NEAREST)
            {
                // �E�[�X�V
                right = middle;
            }
            else if (dot <= -EPS::NEAREST)
            {
                // ���[�X�V
                left = middle;
            }

            // ���S�X�V
            middle = (left + right) / 2.0;

            //// �����X�V�l���[�𒴂�����
            //if (middle < _min_draw_param)
            //    return NearestPointInfoC(GetPositionVector(_min_draw_param), ref, _min_draw_param);
            //if (middle > _max_draw_param)
            //    return NearestPointInfoC(GetPositionVector(_max_draw_param), ref, _max_draw_param);

            // �X�e�b�v������ɒB�����炻�̎��_�̓_��Ԃ�
            if (++count > EPS::COUNT_MAX)
                return NearestPointInfoC(pnt, ref, middle);
        }
    }



    //double left = _min_draw_param;
    //double right = _max_draw_param;
    //double middle = (left + right) / 2;
    //int count = 0;

    //Vector3d pnt = GetPositionVector(middle);
    //Vector3d ref_pnt = pnt - ref;
    //Vector3d tan = GetFirstDiffVector(middle);
    //double dot = tan.Dot(ref_pnt); // ���ϒl

    //                               // �񕪒T��
    //                               // �[�΍�̂��߂ɍX�V�̍�EPS�𑝌�������
    //while (left <= right)
    //{
    //    if (-EPS < dot && dot < EPS)
    //    {
    //        // �\���Ȑ��x�Ȃ̂Ō����������Ƃɂ���
    //        return pnt;
    //    }
    //    else if (dot >= EPS)
    //    {
    //        // �E�[�X�V
    //        right = middle - EPS;
    //    }
    //    else if (dot <= -EPS)
    //    {
    //        // ���[�X�V
    //        left = middle + EPS;
    //    }

    //    // ���S�X�V
    //    middle = (left + right) / 2.0;

    //    // �[��������ԋ߂�
    //    if (middle < _min_draw_param)
    //        return GetPositionVector(_min_draw_param);
    //    if (middle > _max_draw_param)
    //        return GetPositionVector(_max_draw_param);

    //    // �Čv�Z
    //    pnt = GetPositionVector(middle);
    //    ref_pnt = pnt - ref;
    //    tan = GetFirstDiffVector(middle);
    //    dot = tan.Dot(ref_pnt); // ���ϒl

    //    count++;
    //}

    //// ��������
    //return pnt;

    return NearestPointInfoC(pnt, ref, 0);
}