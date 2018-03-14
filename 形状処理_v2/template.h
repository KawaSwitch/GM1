#pragma once

// �`��A�V�X�g
template<class Anything> void DrawWithoutSetting(const Anything& obj)
{
    // �f�o�O�`��p
    obj.Draw();
}

// LU�������ĘA�������������� �W�F�l���b�N��
template <typename T>
vector<T> LUDecomposition(int size, T* aMatrix, T* b)
{
    int N = size; // �����z��̃T�C�Y

                  // L�s��(��s��ɏ�����)
    T **lMatrix = new T*[N];
    for (int i = 0; i < N; i++)
        lMatrix[i] = new T[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            lMatrix[i][j] = 0;
    }

    // U�s��(�Ίp�v�f��1�ɏ�����)
    T **uMatrix = new T*[N];
    for (int i = 0; i < N; i++)
        uMatrix[i] = new T[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            uMatrix[i][j] = (i == j) ? 1 : 0;
    }

    // �v�Z�p�̃o�b�t�@
    T **buffer = new T*[N];
    for (int i = 0; i < N; i++)
        buffer[i] = new T[N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            buffer[i][j] = 0;
    }

    // LU�����J�n
    for (int i = 0; i < N; i++)
    {
        int n = N - i - 1;

        T l0 = lMatrix[i][i] = aMatrix[0];

        // l1�������R�s�[
        T *l1 = new T[n];
        for (int j = 0; j < n; j++)
        {
            lMatrix[j + i + 1][i] = l1[j] = aMatrix[(j + 1) * N + 0];
        }

        // u1^T�������R�s�[
        T *u1 = new T[n];
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
        T **A1 = new T*[n];
        for (int i = 0; i < n; i++)
            A1[i] = new T[n];

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
    T *y = new T[N];
    for (int i = 0; i < N; i++)
    {
        T sum = 0;
        for (int k = 0; k <= i - 1; k++)
        {
            sum += lMatrix[i][k] * y[k];
        }
        y[i] = (b[i] - sum) / lMatrix[i][i];
    }

    vector<T> x; // ���x�N�g��
    x.resize(N);
    for (int i = N - 1; i >= 0; i--)
    {
        T sum = 0;
        for (int j = i + 1; j <= N - 1; j++)
        {
            sum += uMatrix[i][j] * x[j];
        }
        x[i] = y[i] - sum;
    }

    return x;
}

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