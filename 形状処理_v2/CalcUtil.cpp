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

// LU�����ŘA��������������
vector<double> LUDecomposition(int size, double* aMatrix, double* b)
{
    int N = size; // �����z��̃T�C�Y

                  // L�s��(��s��ɏ�����)
    double **lMatrix = new double*[N];
    for (int i = 0; i < N; i++)
        lMatrix[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            lMatrix[i][j] = 0;
    }

    // U�s��(�Ίp�v�f��1�ɏ�����)
    double **uMatrix = new double*[N];
    for (int i = 0; i < N; i++)
        uMatrix[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            uMatrix[i][j] = (i == j) ? 1 : 0;
    }

    // �v�Z�p�̃o�b�t�@
    double **buffer = new double*[N];
    for (int i = 0; i < N; i++)
        buffer[i] = new double[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            buffer[i][j] = 0;
    }

    // LU�����J�n
    for (int i = 0; i < N; i++)
    {
        int n = N - i - 1;

        double l0 = lMatrix[i][i] = aMatrix[0];

        // l1�������R�s�[
        double *l1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            lMatrix[j + i + 1][i] = l1[j] = aMatrix[(j + 1) * N + 0];
        }

        // u1^double�������R�s�[
        double *u1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            uMatrix[i][j + i + 1] = u1[j] = aMatrix[0 * N + (j + 1)] / l0;
        }

        // lu�����߂�
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                buffer[j][k] = l1[j] * u1[k];
            }
        }

        // A1�����߂�
        double **A1 = new double*[n];
        for (int i = 0; i < n; i++)
            A1[i] = new double[n];

        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
                A1[j][k] = aMatrix[(j + 1) * N + (k + 1)] - buffer[j][k];
        }

        // A1��V����aMatrix�Ƃ��ė��p����
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aMatrix[i * N + j] = A1[i][j];
        }
    }

    // LU�s����g���ĘA��������������
    double *y = new double[N];
    for (int i = 0; i < N; i++)
    {
        double sum = 0;
        for (int k = 0; k <= i - 1; k++)
        {
            sum += lMatrix[i][k] * y[k];
        }
        y[i] = (b[i] - sum) / lMatrix[i][i];
    }

    vector<double> x; // ���x�N�g��
    x.resize(N);
    for (int i = N - 1; i >= 0; i--)
    {
        double sum = 0;
        for (int j = i + 1; j <= N - 1; j++)
        {
            sum += uMatrix[i][j] * x[j];
        }
        x[i] = y[i] - sum;
    }

    return x;
}