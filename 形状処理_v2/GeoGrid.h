#pragma once

class GeoGrid2D
{
private:
    int _len; // ���_����̒���
    int _skip; // �O���b�h�Ԃ̒���

    // �����`��
    void DrawInternal()
    {
        glLineWidth(2.0);

        // �������F��ʂɂ��ĕ`��
        glBegin(GL_LINES);
        {
            // x��
            glColor3dv(Color::red);
            glVertex2d(-_len, 0);
            glVertex2d(_len, 0);

            // y��
            glColor3dv(Color::blue);
            glVertex2d(0, -_len);
            glVertex2d(0, _len);
        }
        glEnd();


        // �F�F�O���[
        glColor4d(0.8, 0.8, 0.8, 1.0);

        for (int x = -_len; x <= _len; x += _skip)
        {
            for (int y = -_len; y <= _len; y += _skip)
            {
                glBegin(GL_LINES);
                {
                    // x-direction
                    if (y != 0)
                    {
                        glVertex2d(-_len, y);
                        glVertex2d(_len, y);
                    }

                    // y-direction
                    if (x != 0)
                    {
                        glVertex2d(x, -_len);
                        glVertex2d(x, _len);
                    }
                }
                glEnd();
            }
        }

        glLineWidth(1.0);
    }

public:
    
    GeoGrid2D(int len, int skip) { _len = len; _skip = skip; }
    
    // �f�B�X�v���C���X�g�`��
    void Draw()
    {
        // �f�B�X�v���C���X�g
        static int displayList;
        static bool isRendered = false;

        // �����̓I�t
        if (glIsEnabled(GL_LIGHTING))
            glDisable(GL_LIGHTING);

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

            // actual drawing
            glEnable(GL_DEPTH_TEST);
            this->DrawInternal();

            glEndList();
            isRendered = true;
        }
    }
};
