#pragma once

class Ortho
{
private:

    double _r = 100.0; // �\���͈͂̔��a(�~�j�}�N�X�{�b�N�X)
    double _zoomRatio = 1.0; // �Y�[���{��
    double _aspect; // �A�X�y�N�g��

    double AspectForWidth() { return (_aspect > 1.0) ? _aspect : 1.0; }
    double AspectForHeight() { return (_aspect > 1.0) ? 1.0 : 1.0 / _aspect; }

public:

    double _near, _far;

    double Top() { return _r * _zoomRatio * AspectForHeight(); }
    double Bottom() { return _r * _zoomRatio * AspectForHeight(); }
    double Right() { return _r * _zoomRatio * AspectForWidth(); }
    double Left() { return _r * _zoomRatio * AspectForWidth(); }

    double Width() { return Right() - Left(); }
    double Height() { return Top() - Bottom(); }

    void ZoomIn() { _zoomRatio /= 1.2; }
    void ZoomOut() { _zoomRatio *= 1.2; }

    void SetRadius(double radius) { _r = radius; }
};