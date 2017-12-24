#include "GV.h"
#include "ControlPoint.h"

// 2�������� ax^2 + bx + c = 0 �� 2��x1,x2 �����߂�
// �������̏ꍇ x1 �} x2i �𖞂��� x1,x2 �����߂�
void SolveQuadraticEquation(double a, double b, double c, double* x1, double* x2)
{
    // ���ʎ�
    double D = b * b - 4 * a * c;

    // �قȂ�2�̎�����
    if (D > 0)
    {
        *x1 = (-b + sqrt(D)) / (2 * a);
        *x2 = (-b - sqrt(D)) / (2 * a);
    }
    // �d��
    else if (D == 0)
    {
        // �d���̂Ƃ� x = -(b / 2a)
        *x1 = *x2 = -b / (2 * a);
    }
    // �قȂ�2�̋�����
    else
    {
        *x1 = -b / (2 * a); // real part
        *x2 = sqrt(-D) / (2 * a); // imaginary part
    }
}

// �񍀌W�������߂�
constexpr double Binomial(int n, int k)
{
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

// �K��l���v�Z����
constexpr int Factorial(int n)
{
    return (n == 0) ? 1 : n * Factorial(n - 1);
}

// �o�[���X�^�C�����֐������߂�
double CalcBernsteinFunc(unsigned int i, unsigned int N, double t)
{
    return Binomial(N, i) * pow(t, i) * pow(1 - t, N - i);
}

// 1�K�����p�o�[���X�^�C�����֐������߂�
double Calc1DiffBernsteinFunc(unsigned int i, unsigned int N, double t)
{
    return Binomial(N, i) * (i * pow(t, i - 1) * pow(1 - t, N - i) - (N - i) * pow(t, i) * pow(1 - t, N - i - 1));
}

// 2�K�����p�o�[���X�^�C�����֐������߂�
double Calc2DiffBernsteinFunc(unsigned int i, unsigned int N, double t)
{
    double temp_left = i * ((i - 1) * pow(t, i - 2) * pow(1 - t, N - i)
        - pow(t, i - 1) * (N - i) * pow(1 - t, N - i - 1));

    double temp_right = (N - i) * (i * pow(t, i - 1) * pow(1 - t, N - i - 1)
        - pow(t, i) * (N - i - 1) * pow(1 - t, N - i - 2));

    return Binomial(N, i) * (temp_left - temp_right);
}

// B�X�v���C�����֐��l�����߂�
double CalcBsplineFunc(unsigned int i, unsigned int M, double t, double* knot)
{
    if (M == 1)
    {
        // �[�����͓��ʈ���
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0 , temp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = ((t - knot[i]) / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = ((knot[i + M] - t) / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return temp_left + temp_right;
    }
}

// 1�K�����pB�X�v���C�����֐��l�����߂�
double Calc1DiffBsplineFunc(unsigned int i, unsigned int M, double t, double* knot)
{
    if (M == 1)
    {
        // �[�����͓��ʈ���
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0, temp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = (1 / (knot[i + M - 1] - knot[i])) * CalcBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = (1 / (knot[i + M] - knot[i + 1])) * CalcBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (temp_left - temp_right);
    }
}

// 2�K�����pB�X�v���C�����֐��l�����߂�
double Calc2DiffBsplineFunc(unsigned int i, unsigned int M, double t, double* knot)
{
    if (M == 1)
    {
        // �[�����͓��ʈ���
        if (fabs(t - knot[0]) < 1e-6)
        {
            if (knot[i] <= t && t < knot[i + 1]) return 1.0;
            else return 0.0;
        }
        else
        {
            if (knot[i] < t && t <= knot[i + 1]) return 1.0;
            else return 0.0;
        }
    }
    else
    {
        double temp_left = 0.0, temp_right = 0.0;

        // 0�f�B�o�C�h�ɋC��t����
        if (fabs(knot[i + M - 1] - knot[i]) > 1e-6)
            temp_left = (1 / (knot[i + M - 1] - knot[i])) * Calc1DiffBsplineFunc(i, M - 1, t, knot);

        if (fabs(knot[i + M] - knot[i + 1]) > 1e-6)
            temp_right = (1 / (knot[i + M] - knot[i + 1])) * Calc1DiffBsplineFunc(i + 1, M - 1, t, knot);

        return (M - 1) * (temp_left - temp_right);
    }
}

// ������̒����琔���݂̂����o��
void GetNumberOnly(char *dest, const char *src)
{
    while (*src)
    {
        if ('0' <= *src && *src <= '9') 
            *dest++ = *src;
        
        src++;
        *dest = 0;
    }
}

// 3�_���琬��|���S���̒P�ʉ��ςݖʖ@�����擾����
Vector3d CalcPolygonNormal(Vector3d v0, Vector3d v1, Vector3d v2)
{
    Vector3d vec01 = v1 - v0;
    Vector3d vec02 = v2 - v0;

    return (vec01 * vec02).Normalize();
}

// 3�����_���􉽊w�ϊ�����
Point3d Transform(const Point3d point, const double matrix[16]/* 4 �~ 4�s�� */)
{
    return
    {
        point.X * matrix[0 * 4 + 0] + point.Y * matrix[1 * 4 + 0] + point.Z * matrix[2 * 4 + 0] + matrix[3 * 4 + 0],
        point.X * matrix[0 * 4 + 1] + point.Y * matrix[1 * 4 + 1] + point.Z * matrix[2 * 4 + 1] + matrix[3 * 4 + 1],
        point.X * matrix[0 * 4 + 2] + point.Y * matrix[1 * 4 + 2] + point.Z * matrix[2 * 4 + 2] + matrix[3 * 4 + 2],
        //point.X * matrix[0 * 4 + 0] + point.Y * matrix[0 * 4 + 1] + point.Z * matrix[0 * 4 + 2] + matrix[0 * 4 + 3],
        //point.X * matrix[1 * 4 + 0] + point.Y * matrix[1 * 4 + 1] + point.Z * matrix[1 * 4 + 2] + matrix[1 * 4 + 3],
        //point.X * matrix[2 * 4 + 0] + point.Y * matrix[2 * 4 + 1] + point.Z * matrix[2 * 4 + 2] + matrix[2 * 4 + 3],
    };
}

// 2D:�w����W���S�Ɏw��rad��]������
void RotateCoord2DAroundCenter(double* const coord_2d, const double* const center, const double rad)
{
    // 2 �~ 2�̉�]�s��
    double rotate_mat[2 * 2] =
    {
        cos(rad), -sin(rad),
        sin(rad), cos(rad),
    };

    coord_2d[0] -= center[0];
    coord_2d[1] -= center[1];

    double temp[2];

    MatrixMultiply(2, 2, 1, rotate_mat, coord_2d, temp);

    // ���ʑ��
    coord_2d[0] = temp[0] + center[0];
    coord_2d[1] = temp[1] + center[1];
}

// 2D:���_���S�Ɏw��rad��]������
void RotateCoord2DAroundOrigin(double* const coord_2d, const double rad)
{
    double origin[2] = { 0, 0 }; // ���_
    RotateCoord2DAroundCenter(coord_2d, origin, rad);
}
