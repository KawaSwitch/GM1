#include "GV.h"

// B�X�v���C�����֐��l�����߂�
double CalcBsplineFunc(int i, int M, double t, double* knot)
{
    if (M == 1)
    {
        // �[�����͓��ʈ���
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1])
                return 1.0;
            else
                return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1])
                return 1.0;
            else
                return 0.0;
        }
    }
    else
    {
        double tmp_left = 0.0 , tmp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            tmp_left = ((t - knot[i]) / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            tmp_right = ((knot[i + M] - t) / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return tmp_left + tmp_right;
    }
}

// B�X�v���C�����֐��l
void CalcBsplineFunc2(double t, int mord, int ncpnt, int nknot, double* knot, int reqord, double* bf)
{
    vector<double> basis; // ���֐��l
    vector<vector<double>> bbf; // �e�K�����֐��l�o�b�t�@

    // 1�K�̊��֐��l���擾
    for (int i = 0; i < nknot - 1; i++)
    {
        if ((knot[i] <= t) && (t < knot[i + 1]))
            basis.push_back(1.0);
        else
            basis.push_back(0.0);
    }

    bbf.push_back(basis);
    basis.clear();

    double tmp1 = 0.0;
    double tmp2 = 0.0;

    // 2�K�ȍ~�̊��֐��l���擾
    for (int M = 2; M <= reqord; M++)
    {
        for (int j = 0; j < nknot - M; j++)
        {
            tmp1 = tmp2 = 0.0;

            // ����
            if (knot[j] != knot[j + M - 1])
                tmp1 = (t - knot[j]) / (knot[j + M - 1] - knot[j]) * bbf[M - 1 - 1][j];

            // �E��
            if (knot[j + 1] != knot[j + M])
                tmp2 = (knot[j + M] - t) / (knot[j + M] - knot[j + 1]) * bbf[M - 1 - 1][j + 1];

            // ���v
            basis.push_back(tmp1 + tmp2);
        }

        bbf.push_back(basis);
        basis.clear();
    }

    // return
    for (int i = 0; i < nknot - reqord; i++)
        bf[i] = bbf[reqord - 1][i];
}