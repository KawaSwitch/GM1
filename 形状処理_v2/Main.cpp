#include <gl/freeglut.h>
#include "Callback.h"
#include "Initialize.h"

int main(int argc, char* argv[])
{
    // gl������
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // �E�B���h�E�ݒ�
    glutInitWindowSize(960, 960);
    glutInitWindowPosition(90, 20);
    glutCreateWindow("�`�󏈗�");

    // �e��R�[���o�b�N
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);

    // ������
    Initialize();

    glutMainLoop();
}