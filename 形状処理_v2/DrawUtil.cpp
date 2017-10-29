#include "GV.h"

// ���֐���`�悷��
void DrawBsplineFunc(int mord, int ncpnt, int nknot, double* knot, double min_t, double max_t)
{
    // �f�B�X�v���C���X�g
    static int displayList;
    static bool isRendered = false;

    // �덷�h�~
    int MIN = (int)(min_t * 100);
    int MAX = (int)(max_t * 100);

    glLineWidth(1.5);

    if (isRendered)
    {
        // �f�B�X�v���C���X�g�쐬�ς݂Ȃ�R�[��
        glCallList(displayList);
    }
    else
    {
        if (!(displayList = glGenLists(1)))
            Error::ShowAndExit("�f�B�X�v���C���X�g�쐬���s");

        glNewList(displayList, GL_COMPILE);

        // ���֐��͐���_������
        for (int i = 0; i < ncpnt; i++)
        {
            glBegin(GL_LINE_STRIP);

            // �F�̓����_��
            GLdouble color[4];
            Color::GetRandomColor(color);
            glColor4dv(color);

            for (int T = MIN; T <= MAX; T++)
            {
                double t = (double)T / 100.0;

                double bf = CalcBsplineFunc(i, mord, t, knot);
                glVertex3d(t, bf, 0);
            }

            glEnd();
        }

        glEndList();

        isRendered = true;
    }
}