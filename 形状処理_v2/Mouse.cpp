#define _USE_MATH_DEFINES

#include "GV.h"
#include "Scene.h"
#include "Callback.h"
#include "Quaternion.h"
#include "Picking.h"

extern Scene* scene;
Point3d center;

extern void SetMatrix(Matrix3d);

void Mouse(int button, int state, int x, int y)
{
    // �E�F��]
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        //// �s�N�Z���̃f�v�X�l���擾
        //float z = GetDepth(x, y);

        //// �����ꏊ�̍��W�l���擾
        //center = GetWorldCoord(x, y, z);

        //printf("%f %f %f\n", center.X, center.Y, center.Z);

        //glutPostRedisplay();

        // �}�E�X�{�^�����������ʒu���L��
        mouse_X = x;
        mouse_Y = y;
        rotate_flag = GL_TRUE;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        rotate_flag = GL_FALSE;

        // �ړ����Ă��Ȃ���Εۑ��̕K�v�Ȃ�
        if (x == mouse_X && y == mouse_Y)
            return;

        // �p����ۑ�
        current = target;
    }

    // �z�C�[���F�ړ�
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
    {
        xStart = x;
        yStart = y;
        move_flag = GL_TRUE;
    }
    else
    {
        rotate_flag = GL_FALSE;
        move_flag = GL_FALSE;
        return;
    }
}

void Motion(int x, int y)
{
    if (rotate_flag == GL_FALSE && move_flag == GL_FALSE)
        return;

    if (rotate_flag)
    {
        // �ړ��ʂ��v�Z
        double dx = (x - mouse_X) * 1.33 / width;
        double dy = (y - mouse_Y) * 1.0 / height;

        // �N�H�[�^�j�I���̒���
        double length = sqrt(dx * dx + dy * dy);

        if (length != 0.0)
        {
            double radian = length * M_PI;
            double theta = sin(radian) / length;

            // ��]��̎p��
            Quaternion after = { dy * theta, dx * theta, 0.0, cos(radian) };

            //after = after.Normalize();
            //current = current.Normalize();
            target = after * current;
            //target = target.Normalize();
            //target = after;


            //SetMatrix(scene_mat.RotateAt(after, center));
            CalcRotateMatrix(rot_mat, target);
        }
    }

    if (move_flag)
    {
        double xdis, ydis;
        double a = 0.5; // �����p

        xdis = x - xStart;
        ydis = y - yStart;

        if (move_flag == GL_TRUE)
        {
            dist_X += xdis * a * a * a * a * a;
            dist_Y += ydis * a * a * a * a * a;
        }

        xStart = x;
        yStart = y;

        //SetMatrix(scene_mat.Translate(dist_X, -dist_Y, 0));
    }

    glutPostRedisplay();
}

void Wheel(int wheel_num, int direction, int x, int y)
{
    // �Y�[��
    if (direction == 1)
        dist_Z += 2.0;
    else if (direction == -1)
        dist_Z -= 2.0;

    //SetMatrix(scene_mat.Translate(0, 0, dist_Z));

    glutPostRedisplay();
}