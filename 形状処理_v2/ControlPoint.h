#pragma once

#include "EPS.h"

// ����_�N���X
// �������W�l��
class ControlPoint
{
public:

    double X, Y, Z; // ���W
    double W; // �E�F�C�g

    constexpr ControlPoint() : X(0), Y(0), Z(0), W(1.0) { }
    constexpr ControlPoint(double x, double y, double z) : X(x), Y(y), Z(z), W(1.0) { }
    constexpr ControlPoint(double x, double y, double z, double w) : X(x), Y(y), Z(z), W(w) { }

    bool operator == (const ControlPoint& other)
    {
        return this->GetDistanceTo(other) < EPS::DIST;
    }

    bool operator != (const ControlPoint& other)
    {
        return !(*this == other);
    }

    double GetDistanceTo(ControlPoint other)
    {
        return sqrt(
            (X - other.X)*(X - other.X) +
            (Y - other.Y)*(Y - other.Y) +
            (Z - other.Z)*(Z - other.Z));
    }

    // �x�N�g���v�Z�p�萔�{
    constexpr Vector3d operator*(double value) const
    {
        return { X * value, Y * value, Z * value };
    }
    constexpr Vector3d operator/(double value) const
    {
        return { X / value, Y / value, Z / value };
    }

    friend
        constexpr Vector3d operator*(double value, const ControlPoint& cp);

};

constexpr Vector3d operator*(double value, const ControlPoint& cp)
{
    return { value * cp.X, value * cp.Y, value * cp.Z };
}