#pragma once

#include "LookAt.h"
#include "Ortho.h"
#include "Box.h"

// �J����
class Camera
{
private:

    LookAt _lookAt;
    Ortho _ortho;

    Point3d _rotateCenter; // ��]���S

public:

    Camera()
    {
        
    }
};