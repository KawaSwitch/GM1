#include "GV.h"
#include "Callback.h"
#include "Scene.h"

extern Scene* scene;

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '\033': // �I��
            exit(0);
            break;
        case 'c': // ����_�g�O��

            break;
        default:
            break;
    }

    glutPostRedisplay();
}