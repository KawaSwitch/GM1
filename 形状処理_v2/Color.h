#pragma once
#include "GV.h"

// �F�܂Ƃ�
struct Color
{
public:
    static GLdouble red[];
    static GLdouble green[];
    static GLdouble blue[];
    static GLdouble orange[];
    static GLdouble pink[];
    static GLdouble light_green[];
    static GLdouble light_blue[];

    static GLdouble red_alpha[];
    static GLdouble orange_alpha[];
    static GLdouble pink_alpha[];
    static GLdouble green_alpha[];
    static GLdouble blue_alpha[];

    static GLdouble black[];
    static GLdouble black_alpha[];

public:
    Color() = delete;
};