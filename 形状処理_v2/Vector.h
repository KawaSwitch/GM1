#pragma once
#include <math.h>

// double�^3�����x�N�g���N���X
struct Vector3d
{
    double X, Y, Z;

    Vector3d() { X = 0; Y = 0; Z = 0; } // �f�t�H���g��0������
    Vector3d(double x, double y, double z) { X = x; Y = y; Z = z; }

    // ����
    double Dot(const Vector3d& other) const 
    {
        return
            (X * other.X) + (Y * other.Y) + (Z * other.Z);
    }

    // �O��
    Vector3d Cross(const Vector3d& other) const
    {
        return {
        (Y * other.Z) - (Z * other.Y),
        (Z * other.X) - (X * other.Z),
        (X * other.Y) - (Y * other.X) };
    }

    // ����
    double Length() const { return sqrt(X*X + Y*Y + Z*Z); }
    double LengthSquared() const { return Dot(*this); }
    
    // ��Βl
    double Absolute() const { return Length(); }

    // ����
    double DistanceFrom(const Vector3d& other) const { return (other - *this).Length(); }

    // �P�ʉ�
    Vector3d Normalize() const { return *this / Length(); }

    // 0�m�F
    bool IsZero() const { return (X == 0.0) && (Y == 0.0) && (Z == 0.0); }

    // �R�s�[�R���X�g���N�^�Ƒ�����Z�̓f�t�H���g���g�p


#pragma region operator

    Vector3d operator + () const { return *this; } // �P��+
    Vector3d operator - () const { return { -X, -Y, -Z }; } // �P��-
    // �a
    Vector3d operator+(const Vector3d& other) const
    {
        return { X + other.X, Y + other.Y, Z + other.Z };
    }
    // ��
    Vector3d operator-(Vector3d other) const
    {
        return { X - other.X, Y - other.Y, Z - other.Z };
    }
    // �萔�{
    Vector3d operator*(double value) const
    {
        return { X * value, Y * value, Z * value };
    }
    Vector3d operator/(double value) const
    {
        return{ X / value, Y / value, Z / value };
    }
    // ����������Z
    Vector3d& operator+=(const Vector3d& other)
    {
        X += other.X; Y += other.Y; Z += other.Z;
        return *this;
    }
    Vector3d& operator-=(const Vector3d& other)
    {
        X -= other.X; Y -= other.Y; Z -= other.Z;
        return *this;
    }
    Vector3d& operator*=(double value)
    {
        X *= value; Y *= value; Z *= value;
        return *this;
    }
    Vector3d& operator/=(double value)
    {
        X /= value; Y /= value; Z /= value;
        return *this;
    }

#pragma endregion
};