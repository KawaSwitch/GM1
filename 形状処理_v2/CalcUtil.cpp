#include "GV.h"
#include "ControlPoint.h"

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

// 1�K�����pB�X�v���C�����֐��l�����߂�
double Calc1DiffBsplineFunc(int i, int M, double t, double* knot)
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
        double tmp_left = 0.0, tmp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            tmp_left = (1 / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            tmp_right = (1 / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (tmp_left - tmp_right);
    }
}

// 2�K�����pB�X�v���C�����֐��l�����߂�
double Calc2DiffBsplineFunc(int i, int M, double t, double* knot)
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
        double tmp_left = 0.0, tmp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            tmp_left = (1 / (knot[i + M - 1] - knot[i])) * Calc1DiffBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            tmp_right = (1 / (knot[i + M] - knot[i + 1])) * Calc1DiffBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (tmp_left - tmp_right);
    }
}