#pragma once

#include "GV.h"
#include "Object.h"

// ���̕\���^�C�v
enum AxisShowType
{
    None,  // �\�����Ȃ�
    Normal, // ���ʂɕ`��
    WithLabel, // XYZ���x�����`��
};

extern AxisShowType axisShowType;

class Axis
{
protected:
    double _length; // ���̕����̒���

public:
    Axis(double len) { _length = len; }

    // ��󕔕����܂߂�������Ԃ�
    double GetLength() { return _length + _length / 4; }
};

// �\���b�h��
class NormalAxis : public Axis
{
private:
    void DrawInternal();
    void DrawCharacter();

public:
    NormalAxis(double len) : Axis(len) { }
    void Draw();
};