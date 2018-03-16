#pragma once

#include "GV.h"
#include "Object.h"

class Axis
{
protected:
    double _length; // ���̕����̒���

public:
    Axis(double len) { _length = len; }

    // ��󕔕����܂߂�������Ԃ�
    double GetLength() { return _length + _length / 4; }
};

// Cggl���̎�
class NormalAxis : public Axis
{
private:
    void DrawInternal();

public:
    NormalAxis(double len) : Axis(len) { }
    void Draw();
};