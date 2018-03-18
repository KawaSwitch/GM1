#include "Curve.h"

// ����_�ݒ�
void Curve::SetControlPoint(ControlPoint* cp, int size)
{
    if (size <= 0)
        Error::ShowAndExit("����_�ݒ莸�s", "CP size must be over 0.");

    _ctrlp.reserve(size);

    for (int i = 0; i < size; i++)
        _ctrlp.emplace_back(cp[i]);
}

// ����_�`��
void Curve::DrawCPsInternal()
{
    //glColor3d(1.0, 0.0, 0.0); // ��
    glPointSize(10.0);
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

// �ȗ��x�N�g���擾
Vector3d Curve::GetCurvatureVector(double t)
{
    double kappa =  // �� = |Pt�~Ptt| / |Pt|^3
        (GetFirstDiffVector(t) * GetSecondDiffVector(t)).Length() // |Pt�~Ptt|
        / pow(GetFirstDiffVector(t).Length(), 3); // |Pt|^3

    // �@������N = (Pt �~ Ptt) �~ Pt
    Vector3d direct = (GetFirstDiffVector(t) * GetSecondDiffVector(t)) * GetFirstDiffVector(t);

    return (1 / kappa) * direct.Normalize();
}

// �`��͈͂�split_num�ɕ�������悤�Ȉʒu�x�N�g�����擾����
vector<Vector3d> Curve::GetPositionVectors(int split_num)
{
    vector<Vector3d> pos;

    // ������Ԃ��v�Z
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / split_num;

    // double�^�̌덷�l��
    for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
        pos.push_back(GetPositionVector(t));

    return pos;
}

// �Q�Ɠ_����̍ŋߓ_���擾����
Vector3d Curve::GetNearestPointFromRef(Vector3d ref)
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

// ���Ȑ��Ƃ̑���x���v�Z���܂�
double Curve::CalcDifferency(Curve* other)
{
    int checkCnt = 100; // �����𑪂�_�̐�
    double sumDistance = 0.0; // ���዗���̍��v
    double minParam[2], maxParam[2]; // �[�̃p�����[�^

    minParam[0] = this->GetMinDrawParam();
    maxParam[0] = this->GetMaxDrawParam();
    minParam[1] = other->GetMinDrawParam();
    maxParam[1] = other->GetMaxDrawParam();

    // ������Ԃ��v�Z
    double skip = (fabs(_min_draw_param) + fabs(_max_draw_param)) / checkCnt;

    // double�^�̌덷�l��
    for (double t = _min_draw_param; t < _max_draw_param + skip / 2; t += skip)
        sumDistance += this->GetPositionVector(t).DistanceFrom(other->GetPositionVector(t));
    // ���዗���̕��ς�Ԃ�
    return sumDistance / (double)checkCnt;
}