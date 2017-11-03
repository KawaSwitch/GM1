#pragma once

#include "GV.h"
#include "ControlPoint.h"

// 曲線インターフェース
class Curve : public Object
{
protected:

    int _ord; // 階数
    int _ncpnt; // 制御点数
    vector<ControlPoint> _ctrlp; // 制御点

    // 制御点設定
    void SetControlPoint(ControlPoint* cp, int size)
    {
        if (size <= 0)
            Error::ShowAndExit("制御点設定失敗", "CP size must be over 0.");

        _ctrlp.reserve(size);

        for (int i = 0; i < size; i++)
            _ctrlp.emplace_back(cp[i]);
    }

    virtual Vector3d GetPositionVector(double t) = 0; // 位置ベクトル
    virtual Vector3d GetFirstDiffVector(double t) = 0; // 接線ベクトル

private:

    // 制御点描画
    void DrawCPsInternal() override
    {
        glColor3d(1.0, 0.0, 0.0); // 赤
        glPointSize(5.0);
        glLineWidth(1.0);

        // 点群
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();

        // 線群
        glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < _ctrlp.size(); i++)
            glVertex3d(_ctrlp[i].X, _ctrlp[i].Y, _ctrlp[i].Z);
        glEnd();
    }

public:

    virtual ~Curve() { };
};