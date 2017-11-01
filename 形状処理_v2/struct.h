#pragma once

#include "define.h"

// �N�H�[�^�j�I��
struct Quaternion
{
    double w;
    double x;
    double y;
    double z;
};

// 2�����_
template<class T>
struct Point
{
    T x;
    T y;
};

// �I���f�[�^
struct SelectionData
{
    unsigned int minZ;
    unsigned int maxZ;
    unsigned int name;

    SelectionData() { minZ = 0; maxZ = 0; name = 0; }
    SelectionData(unsigned int m, unsigned int M, unsigned int n)
    {
        minZ = m;
        maxZ = M;
        name = n;
    }
};