#pragma once

template<class GeoGrid> void DrawGrid(const GeoGrid& grid)
{
    // �􉽃O���b�h�`��
    grid.Draw();
}

// 2D�O���b�h
struct GeoGrid2D
{
    void Draw() const
    {
        // �F�F�O���[
        glColor4d(0.8, 0.8, 0.8, 1.0);

        for (double x = -50; x <= 50; x += 1)
        {
            for (double y = -50; y <= 50; y += 1)
            {
                glBegin(GL_LINES);

                // x-direction
                glVertex3d(-50, y, 0);
                glVertex3d(50, y, 0);

                // y-direction
                glVertex3d(x, -50, 0);
                glVertex3d(x, 50, 0);

                glEnd();
            }
        }
    }
};

// 3D�O���b�h
struct GeoGrid3D
{
    void Draw() const
    {

    }
};