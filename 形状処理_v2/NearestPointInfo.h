#pragma once

#include "Vector.h"

// �ŋߓ_���܂Ƃߍ\����
struct NearestPointInfo
{
    Vector3d nearestPnt; // �ŋߓ_
    double dist; // �ŋߓ_����Q�Ɠ_�܂ł̋���

    NearestPointInfo(Vector3d nearest, Vector3d ref)
    {
        nearestPnt = nearest;
        dist = nearest.DistanceFrom(ref);
    }
};

// �ŋߓ_���܂Ƃߍ\���� �Ȑ��p
struct NearestPointInfoC : public NearestPointInfo
{
public:

    double param; // �p�����[�^�ʒu

    NearestPointInfoC(Vector3d nearest, Vector3d ref, double t)
        : NearestPointInfo(nearest, ref)
    {
        param = t;
    }
};

// �ŋߓ_���܂Ƃߍ\���� �Ȗʗp
struct NearestPointInfoS : public NearestPointInfo
{
public:

    double paramU, paramV; // �p�����[�^�ʒu

    NearestPointInfoS(Vector3d nearest, Vector3d ref, double u, double v)
        : NearestPointInfo(nearest, ref)
    {
        paramU = u;
        paramV = v;
    }
};