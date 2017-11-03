#include "GV.h"
#include "Callback.h"
#include "Scene.h"

extern Scene* scene;

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '\033': // 終了
            exit(0);
            break;
        case 'c': // 制御点トグル
            scene->ToggleDrawControlPoints();
            break;
        case 'f': // 接線トグル
            scene->ToggleDrawFirstDiffVectors();
        default:
            break;
    }

    glutPostRedisplay();
}