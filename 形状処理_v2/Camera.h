#pragma once

#include "LookAt.h"
#include "Ortho.h"
#include "Box.h"

// カメラ
class Camera
{
private:

    LookAt _lookAt;
    Ortho _ortho;

    Point3d _rotateCenter; // 回転中心

public:

    Camera()
    {
        
    }
};