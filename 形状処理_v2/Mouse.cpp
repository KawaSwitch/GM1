#define _USE_MATH_DEFINES

#include "GV.h"
#include "Scene.h"
#include "Callback.h"
#include "Quaternion.h"
#include "Picking.h"

extern Scene* scene;

extern void SetMatrix(Matrix3d);

void Mouse(int button, int state, int x, int y)
{
    // 右：回転
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
#ifdef _DEBUG
        // ピクセルのデプス値を取得
        float z = GetDepth(x, y);

        // 押下場所の座標値を取得
        auto coord = GetWorldCoord(x, y, z);

        printf("mouse : x, y, depth : %d %d %f\n", x, y, z);
        printf("world : x, y, z : %f %f %f\n\n", coord.X, coord.Y, coord.Z);
#endif

        // マウスボタンを押した位置を記憶
        mouse_X = x;
        mouse_Y = y;
        rotate_flag = GL_TRUE;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        rotate_flag = GL_FALSE;

        // 移動していなければ保存の必要なし
        if (x == mouse_X && y == mouse_Y)
            return;

        // 姿勢を保存
        current = target;
    }

    // 左：移動
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        xStart = x;
        yStart = y;
        move_flag = GL_TRUE;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        move_flag = GL_FALSE;
    }

    // それ以外はどちらでもない
    else
    {
        rotate_flag = GL_FALSE;
        move_flag = GL_FALSE;
    }

    glutPostRedisplay();
}

void Motion(int x, int y)
{
    if (rotate_flag == GL_FALSE && move_flag == GL_FALSE)
        return;

    if (rotate_flag)
    {
        // 移動量を計算
        double dx = (x - mouse_X) * 1.33 / width;
        double dy = (y - mouse_Y) * 1.0 / height;

        // クォータニオンの長さ
        double length = sqrt(dx * dx + dy * dy);

        if (length != 0.0)
        {
            double radian = length * M_PI;
            double theta = sin(radian) / length;

            // 回転後の姿勢
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
        double a = 0.5; // 調整用

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
    // ズーム
    if (direction == 1)
        dist_Z += 2.0;
    else if (direction == -1)
        dist_Z -= 2.0;

    //SetMatrix(scene_mat.Translate(0, 0, dist_Z));

    glutPostRedisplay();
}