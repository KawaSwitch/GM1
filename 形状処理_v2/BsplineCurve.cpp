#include "BsplineCurve.h"

BsplineCurve::BsplineCurve(int mord, ControlPoint* cp, int cp_size, double* knot, GLdouble* color, GLdouble width)
{
    _ord = mord;
    _ncpnt = cp_size;
    _nknot = mord + cp_size;

    SetControlPoint(cp, cp_size);
    SetKnotVector(knot, _nknot);
    SetColor(color);

    // VBO�g��
    _isUseVBO = true;
}

// ���O�`��
void BsplineCurve::PreDraw()
{
    glBegin(GL_LINE_STRIP);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;

        Vector3d pnt = GetPositionVector(t);
        glVertex3d(pnt);
    }

    glEnd();
}

// ���_�o�b�t�@�쐬
void BsplineCurve::CreateVBO()
{
    vector<Vector3d> pnts;

    // ���_�擾
    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i++)
    {
        double t = (double)i / 100;
        pnts.push_back(GetPositionVector(t));
    }

    _nVertex = (int)pnts.size();

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, pnts.size() * 3 * sizeof(double), (GLdouble*)&pnts[0], GL_DYNAMIC_DRAW);
}

// VBO�ŕ`��
void BsplineCurve::DrawVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_LINE_STRIP, 0, _nVertex);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// �ڐ��x�N�g���`��
void BsplineCurve::DrawFirstDiffVectors()
{
    if (_isDrawFirstDiff)
        DrawUsingDisplayList(&_fd_displayList, [&] { return (*this).DrawFirstDiffVectorsInternal(); });
}
void BsplineCurve::DrawFirstDiffVectorsInternal()
{
    glColor3dv(Color::red);
    glLineWidth(1.0);
    glBegin(GL_LINES);

    for (int i = (int)(_knot[_ord - 1] * 100); i <= (int)(_knot[_ncpnt] * 100); i += 10)
    {
        double t = (double)i / 100;

        Vector3d pnt = GetPositionVector(t);
        Vector3d diff = GetFirstDiffVector(t).Normalize();
        glVertex3d(pnt);
        glVertex3d(pnt + diff);
    }

    glEnd();
}

// �ʒu�x�N�g���擾
Vector3d BsplineCurve::GetPositionVector(double t)
{
    Vector3d pnt;

    for (int i = 0; i < _ncpnt; i++)
    {
        double N = CalcBsplineFunc(i, _ord, t, &_knot[0]);

        pnt.X += N * _ctrlp[i].X;
        pnt.Y += N * _ctrlp[i].Y;
        pnt.Z += N * _ctrlp[i].Z;
    }

    return pnt;
}

// �ڐ��x�N�g���擾
Vector3d BsplineCurve::GetFirstDiffVector(double t)
{
    Vector3d diff;

    for (int i = 0; i < _ncpnt; i++)
    {
        double N = Calc1DiffBsplineFunc(i, _ord, t, &_knot[0]);

        diff.X += N * _ctrlp[i].X;
        diff.Y += N * _ctrlp[i].Y;
        diff.Z += N * _ctrlp[i].Z;
    }

    return diff;
}