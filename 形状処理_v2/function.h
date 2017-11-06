#pragma once
#include <vector>

// Initialize.cpp
    GLOBAL void Initialize();

// CalcUtil.cpp
    GLOBAL void SolveQuadraticEquation(double a, double b, double c, double* x1, double* x2);
    GLOBAL double CalcBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double Calc1DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double Calc2DiffBernsteinFunc(unsigned int i, unsigned int N, double t);
    GLOBAL double CalcBsplineFunc(unsigned int i, unsigned int M, double t, double* knot);
    GLOBAL double Calc1DiffBsplineFunc(unsigned int i, unsigned int M, double t, double* knot);
    GLOBAL double Calc2DiffBsplineFunc(unsigned int i, unsigned int M, double t, double* knot);
    GLOBAL constexpr double Binomial(int n, int k);
    GLOBAL constexpr int Factorial(int n);
    GLOBAL Vector3d CalcPolygonNormal(Vector3d v0, Vector3d v1, Vector3d v2);
    GLOBAL void GetNumberOnly(char *dest, const char *src);

// DrawUtil.cpp
    GLOBAL void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t = 0.0, double max_t = 1.0);
    GLOBAL void DrawUsingDisplayList(int* const displayList, function<void(void)> RegisterDraw);

// glOverload.cpp
    GLOBAL void glVertex3d(const Vector3d& vec);
    GLOBAL void glPointSize(const double size);
    GLOBAL void glLineWidth(const double width);
    GLOBAL void glNormal3d(const Vector3d& vec);
    GLOBAL void glMaterialfv(GLenum face, GLenum pname, const GLdouble* params);


/* ----------�e���v���[�g---------- */
// LU������N���A��������������
template <int N>
vector<double> LUDecomposition(double aMatrix[N][N], double b[N])
{
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

        double l0 = lMatrix[i][i] = aMatrix[0][0];

        // l1�������R�s�[
        double *l1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            lMatrix[j + i + 1][i] = l1[j] = aMatrix[j + 1][0];
        }

        // u1^T�������R�s�[
        double *u1 = new double[n];
        for (int j = 0; j < n; j++)
        {
            uMatrix[i][j + i + 1] = u1[j] = aMatrix[0][j + 1] / l0;
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
                A1[j][k] = aMatrix[j + 1][k + 1] - buffer[j][k];
        }

        // A1��V����aMatrix�Ƃ��ė��p����
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aMatrix[i][j] = A1[i][j];
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
