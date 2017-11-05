#include "GV.h"
#include "Callback.h"
#include "Scene.h"

extern Scene* scene;
extern Scene* test_scene;

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '\033': // �I��
            exit(0);
            break;
        case 'p': // ����_�g�O��
            scene->ToggleDrawControlPoints();
            test_scene->ToggleDrawControlPoints();
            break;
        case 'f': // �ڐ��g�O��
            scene->ToggleDrawFirstDiffVectors();
            test_scene->ToggleDrawFirstDiffVectors();
            break;
        case 's': // 2�K�����x�N�g���g�O��
            scene->ToggleDrawSecondDiffVectors();
            test_scene->ToggleDrawSecondDiffVectors();
            break;
        case 'n': // �@���g�O��
            scene->ToggleDrawNormalVectors();
            test_scene->ToggleDrawNormalVectors();
            break;
        case 'c': // �ȗ��x�N�g���g�O��
            scene->ToggleDrawCurvatureVectors();
            test_scene->ToggleDrawCurvatureVectors();
            break;
        case 'b': // �~�j�}�N�X�{�b�N�X�g�O��
            scene->ToggleDrawBox();
            test_scene->ToggleDrawBox();
            break;
        //case 'l': // ���C�e�B���O�g�O��
        //    isUseLight = !isUseLight;
        //    break;
        default:
            break;
    }

    glutPostRedisplay();
}