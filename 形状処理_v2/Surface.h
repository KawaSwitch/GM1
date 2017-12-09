#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "Curve.h"

extern Scene* test_scene;

// �ȖʃC���^�[�t�F�[�X
class Surface : public Object
{
protected:

    int _ordU, _ordV; // �K��
    int _nVertex; // ���_��
    GLuint _vbo_nor = 0; // �@���pvbo
    int _ncpntU, _ncpntV; // ����_��
    vector<double> _ctrlpX; // �v�Z�p
    vector<double> _ctrlpY;
    vector<double> _ctrlpZ;
    double _min_draw_param_U, _max_draw_param_U; // �`��͈̓p�����[�^
    double _min_draw_param_V, _max_draw_param_V;
    int _mesh_displayList = 0; // ���b�V���p�f�B�X�v���C���X�g
    double _mesh_width; // ���b�V�����̕�

    // ����_�ݒ�
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

        _ctrlp.reserve(size); _ctrlpX.reserve(size);
        _ctrlpY.reserve(size); _ctrlpZ.reserve(size);

        for (int i = 0; i < size; i++)
        {
            _ctrlp.emplace_back(cp[i]);

            //// �]�u�Ȃ�
            //_ctrlpX.emplace_back(cp[i].X); 
            //_ctrlpY.emplace_back(cp[i].Y);
            //_ctrlpZ.emplace_back(cp[i].Z);
        }

        // �v�Z�p�ɓ]�u����(�����ƌ��h���������������~����)
        for (int i = 0; i < _ncpntU; i++)
        {
            for (int j = 0; j < _ncpntV; j++)
            {
                _ctrlpX.push_back(_ctrlp[j * _ncpntU + i].X);
                _ctrlpY.push_back(_ctrlp[j * _ncpntU + i].Y);
                _ctrlpZ.push_back(_ctrlp[j * _ncpntU + i].Z);
            }
        }
    }

    // ���b�V���\��
    virtual void DrawMeshInternal() { };

    void DrawMesh()
    {
        if (!isUseLight) // ���������Ȃ��̏ꍇ�͌��h���̂��߃��b�V���\������
            DrawUsingDisplayList(&_mesh_displayList, [&] { return (*this).DrawMeshInternal(); });
    }

    // �x�N�g���擾�֐�
    virtual Vector3d GetPositionVector(double u, double v) = 0; // �ʒu�x�N�g��
    virtual Vector3d GetFirstDiffVectorU(double u, double v) = 0; // �ڐ��x�N�g��
    virtual Vector3d GetFirstDiffVectorV(double u, double v) = 0;
    virtual Vector3d GetSecondDiffVectorUU(double u, double v) { return Vector3d(); }; // 2�K�����x�N�g��
    virtual Vector3d GetSecondDiffVectorUV(double u, double v) { return Vector3d(); };
    virtual Vector3d GetSecondDiffVectorVV(double u, double v) { return Vector3d(); };

    // �@���x�N�g���擾
    Vector3d GetNormalVector(double u, double v)
    {
        return (GetFirstDiffVectorU(u, v) * GetFirstDiffVectorV(u, v));
    }

    // �`��p�ȗ��x�N�g���擾
    Vector3d GetCurvatureVector(double u, double v)
    {
        // ��ȗ��擾
        double max_kappa, min_kappa;
        GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

        // �P�ʖ@���x�N�g��
        Vector3d e = GetNormalVector(u, v).Normalize();

        // ��Βl���傫������Ԃ�
        return (fabs(max_kappa) > fabs(min_kappa)) ?
            (1 / max_kappa) * e :
            (1 / min_kappa) * e;
    }

    // �w�肵���[�̋Ȑ��̐���_���擾����
    vector<ControlPoint> GetEdgeCurveControlPoint(SurfaceEdge edge)
    {
        vector<ControlPoint> edge_cp;

        if (edge == SurfaceEdge::U_min)
        {
            for (int i = 0; i <_ncpntU * _ncpntV; i += _ncpntU)
                edge_cp.push_back(_ctrlp[i]);
        }
        else if (edge == SurfaceEdge::U_max)
        {
            for (int i = _ncpntU - 1; i < _ncpntU * _ncpntV; i += _ncpntU)
                edge_cp.push_back(_ctrlp[i]);
        }
        else if (edge == SurfaceEdge::V_min)
        {
            for (int i = 0; i < _ncpntU; i++)
                edge_cp.push_back(_ctrlp[i]);
        }
        else if (edge == SurfaceEdge::V_max)
        {
            for (int i = _ncpntU * (_ncpntV - 1) - 1; i < _ncpntU * _ncpntV; i++)
                edge_cp.push_back(_ctrlp[i]);
        }

        return edge_cp;
    }

private:

    // ��ȗ����擾
    void GetPrincipalCurvatures(double u, double v, double* max_kappa, double* min_kappa)
    {
        Vector3d e = GetNormalVector(u, v).Normalize(); // �P�ʖ@���x�N�g��

        // �e���K�v�Ȓl���v�Z
        double L = e.Dot(GetSecondDiffVectorUU(u, v));
        double M = e.Dot(GetSecondDiffVectorUV(u, v));
        double N = e.Dot(GetSecondDiffVectorVV(u, v));
        double E = GetFirstDiffVectorU(u, v).Dot(GetFirstDiffVectorU(u, v));
        double F = GetFirstDiffVectorU(u, v).Dot(GetFirstDiffVectorV(u, v));
        double G = GetFirstDiffVectorV(u, v).Dot(GetFirstDiffVectorV(u, v));

        double A = E * G - F * F;
        double B = 2 * F * M - E * N - G * L;
        double C = L * N - M * M;

        // ��ȗ��Ȃ��v�Z����iA��^2 + B�� + C = 0 ��2���j
        double kappa1, kappa2;
        SolveQuadraticEquation(A, B, C, &kappa1, &kappa2);

        // �ő��ȗ��ƍŏ���ȗ���Ԃ�
        *max_kappa = (kappa1 > kappa2) ? kappa1 : kappa2;
        *min_kappa = (kappa1 < kappa2) ? kappa1 : kappa2;
    }

    // ���ϋȗ��擾
    double GetMeanCurvature(double u, double v)
    {
        // ��ȗ��擾
        double max_kappa, min_kappa;
        GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

        return (max_kappa + min_kappa) / 2.0;
    }

    // �K�E�X�ȗ��擾
    double GetGaussianCurvature(double u, double v)
    {
        // ��ȗ��擾
        double max_kappa, min_kappa;
        GetPrincipalCurvatures(u, v, &max_kappa, &min_kappa);

        return max_kappa * min_kappa;
    }

public:

    // �I�u�W�F�N�g�`��
    void Draw() override
    {
        if (isUseLight)
            glEnable(GL_LIGHTING);

        if (_isUseVBO)
        {
            // VBO
            if (_vbo == 0)
            {
                CreateVBO(); // VBO�쐬
                glutPostRedisplay();
            }
            else
                DrawVBO(); // �`��
        }
        else
        {
            // �f�B�X�v���C���X�g
            DrawUsingDisplayList(&_displayList, [&] { return (*this).PreDraw(); });
        }

        glDisable(GL_LIGHTING);
    }

    // ����_�`��
    void DrawCPsInternal() override
    {
        glColor3d(1.0, 0.0, 0.0); // ��
        glPointSize(5.0);
        glLineWidth(1.0);

        glDisable(GL_DEPTH_TEST);

        // �_�Q
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();

        // U�������Q
        for (int i = 0; i < _ncpntV; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < _ncpntU; j++)
                glVertex3d(_ctrlp[i * _ncpntU + j].X, _ctrlp[i * _ncpntU + j].Y, _ctrlp[i * _ncpntU + j].Z);
            glEnd();
        }
        // V�������Q
        for (int i = 0; i < _ncpntU; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < _ncpntV; j++)
                glVertex3d(_ctrlp[j * _ncpntU + i].X, _ctrlp[j * _ncpntU + i].Y, _ctrlp[j * _ncpntU + i].Z);
            glEnd();
        }

        glEnable(GL_DEPTH_TEST);
    }

    // �w�肵���p�����[�^�̃A�C�\�Ȑ����擾����
    //virtual Curve* GetIsoCurve(ParamUV direct, double param) = 0;

    // �w�肵���[�̋Ȑ����擾����
    virtual Curve* GetEdgeCurve(SurfaceEdge edge) = 0;

    // �Q�Ɠ_����̍ŋߓ_���擾(�A�C�\�����Ȃ��̂ō��͋Ȗʂ���͂ݏo�Ȃ��͈͂̂݉\)
    Vector3d GetNearestPointFromRef(Vector3d ref)
    {
        int count = 0;

        // ���e�l
        const double EPS = 10e-6;

        // �����p�����[�^ �Ƃ肠�������S��
        double u = (_min_draw_param_U + _max_draw_param_U) / 2.0;
        double v = (_min_draw_param_V + _max_draw_param_V) / 2.0;

        // ���ˉe�@(DAP�@)
        Vector3d p = GetPositionVector(u, v);
        Vector3d pu = GetFirstDiffVectorU(u, v);
        Vector3d pv = GetFirstDiffVectorV(u, v);

        double delta_u = (ref - p).Dot(pu) / pow(pu.Length(), 2.0) * 0.5; // 0.7�|�����Ƃ��������Ƃ�������
        double delta_v = (ref - p).Dot(pv) / pow(pv.Length(), 2.0) * 0.5; // CGS_bspline_surface_1.kjs

        while (fabs(delta_u) > EPS || fabs(delta_v) > EPS)
        {
            u += delta_u;
            v += delta_v;

            // ���ڃx�N�g����Pu�����p ���� ���ڃx�N�g����Pv�����p
            if (fabs((ref - p).Dot(pu)) < EPS && fabs((ref - p).Dot(pv) < EPS))
                break;

            // u, v�����Ƃ��ɂ͂ݏo��ꍇ���Е��̔��肾���ŏ\��
            // u�������͂ݏo��ꍇ
            if (u < _min_draw_param_U || u > _max_draw_param_U)
            {
                if (u < _min_draw_param_U)
                {
                    Curve* edge = GetEdgeCurve(SurfaceEdge::U_min);
                    return edge->GetNearestPointFromRef(ref);
                }
                else
                {
                    Curve* edge = GetEdgeCurve(SurfaceEdge::U_max);
                    return edge->GetNearestPointFromRef(ref);
                }
            }
            // v�������͂ݏo��ꍇ
            else if (v < _min_draw_param_V || v > _max_draw_param_V)
            {
                if (v < _min_draw_param_V)
                {
                    Curve* edge = GetEdgeCurve(SurfaceEdge::V_min);
                    return edge->GetNearestPointFromRef(ref);
                }
                else
                {
                    Curve* edge = GetEdgeCurve(SurfaceEdge::V_max);
                    return  edge->GetNearestPointFromRef(ref);
                }
            }

            p = GetPositionVector(u, v);
            pu = GetFirstDiffVectorU(u, v);
            pv = GetFirstDiffVectorV(u, v);

            delta_u = (ref - p).Dot(pu) / pow(pu.Length(), 2.0) * 0.7; // �X�V��0.7�ł����Ȃ�����
            delta_v = (ref - p).Dot(pv) / pow(pv.Length(), 2.0) * 0.7; // CGS_bspline_surface_1.kjs

            // �J��Ԃ������\���Ȃ�ŋߓ_�Ƃ���
            if (count++ > 1000)
                break;
        }

        return GetPositionVector(u, v);
    }

    virtual ~Surface() { glDeleteLists(_mesh_displayList, 1); }
};