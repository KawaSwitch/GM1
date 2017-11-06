#pragma once

#include "Object.h"
#include "ControlPoint.h"

// �Ȑ��C���^�[�t�F�[�X
class Curve : public Object
{
protected:

    int _ord; // �K��
    int _ncpnt; // ����_��
    int _nVertex; // ���_��
    double _width; // �Ȑ��̕�
    double _min_draw_param, _max_draw_param; // �`��͈̓p�����[�^

    // �x�N�g���擾�֐�
    virtual Vector3d GetPositionVector(double t) = 0; // �ʒu�x�N�g��
    virtual Vector3d GetFirstDiffVector(double t) = 0; // �ڐ��x�N�g��
    virtual Vector3d GetSecondDiffVector(double t) = 0; // 2�K�����x�N�g��

    // �@���x�N�g���擾
    Vector3d GetNormalVector(double t)
    {
        return (Vector3d(0, 0, 1) * GetFirstDiffVector(t));
    }

    // �ȗ��x�N�g���擾
    Vector3d GetCurvatureVector(double t)
    {
        double kappa =  // �� = |Pt�~Ptt| / |Pt|^3
            (GetFirstDiffVector(t) * GetSecondDiffVector(t)).Length() // |Pt�~Ptt|
            / pow(GetFirstDiffVector(t).Length(), 3); // |Pt|^3

        // �@������N = (Pt �~ Ptt) �~ Pt
        Vector3d direct = (GetFirstDiffVector(t) * GetSecondDiffVector(t)) * GetFirstDiffVector(t);

        return (1 / kappa) * direct.Normalize();
    }

    // ����_�ݒ�
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

        _ctrlp.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);
    }

private:

    // ����_�`��
    void DrawCPsInternal() override
    {
        glColor3d(1.0, 0.0, 0.0); // ��
        glPointSize(5.0);
        glLineWidth(1.0);

        // �_�Q
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();

        // ���Q
        glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();
    }

public:

    // �Q�Ɠ_����̍ŋߓ_���擾����
    Vector3d GetNearestPointFromRef(Vector3d ref)
    {
        const double EPS = 10e-12;
        double left = _min_draw_param;
        double right = _max_draw_param;
        double middle = (left + right) / 2;
        int count = 0;

        Vector3d pnt = GetPositionVector(middle);
        Vector3d ref_pnt = pnt - ref;
        Vector3d tan = GetFirstDiffVector(middle);
        double dot = tan.Dot(ref_pnt); // ���ϒl

        // �񕪒T��
        // �[�΍�̂��߂ɍX�V�̍�EPS�𑝌�������
        while (left <= right)
        {
            if (-EPS < dot && dot < EPS)
            {
                // �\���Ȑ��x�Ȃ̂Ō����������Ƃɂ���
                return pnt;
            }
            else if (dot >= EPS)
            {
                // �E�[�X�V
                right = middle - EPS;
            }
            else if (dot <= -EPS)
            {
                // ���[�X�V
                left = middle + EPS;
            }

            // ���S�X�V
            middle = (left + right) / 2.0;

            // �[��������ԋ߂�
            if (middle < _min_draw_param)
                return GetPositionVector(_min_draw_param);
            if (middle > _max_draw_param)
                return GetPositionVector(_max_draw_param);
                
            // �Čv�Z
            pnt = GetPositionVector(middle);
            ref_pnt = pnt - ref;
            tan = GetFirstDiffVector(middle);
            dot = tan.Dot(ref_pnt); // ���ϒl

            count++;
        }

        // ��������
        return pnt;
    }

    // �`��͈͂�split_num�ɕ�������悤�Ȉʒu�x�N�g�����擾����
    vector<Vector3d> GetPositionVectors(int split_num)
    {
        vector<Vector3d> pos;

        // ������Ԃ��v�Z
        double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / split_num;
      
        // double�^�̌덷�l��
        for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
            pos.push_back(GetPositionVector(t));

        return pos;
    }

    virtual ~Curve() { };
};