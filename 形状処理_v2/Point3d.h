#pragma once

// Vector3d�̎c�[ �ꉞ�c���Ă�
class Point3d
{
    double X, Y, Z;

    operator Point3d()
    {
        return Point3d(X, Y, Z);
    }

    Point3d() { X = 0; Y = 0; Z = 0; }
    Point3d(double x, double y, double z) { X = x; Y = y; Z = z; }
};