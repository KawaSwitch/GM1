#pragma once

#include "GV.h"
#include "Object.h"

class Axis
{
protected:
    double _length; // ����

public:
    Axis(double len) { _length = len; }
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