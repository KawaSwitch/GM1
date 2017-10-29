#pragma once

#include "GV.h"
#include "Object.h"

class Axis : public Object
{
protected:
    double _length; // ����
    //double _weight; // ����

public:
    Axis(double len) { _length = len; }
};

class NormalAxis : public Axis
{
private:
    void DrawInternal();

public:
    NormalAxis(double len) : Axis(len) { }
    virtual void PreDraw() override;
};