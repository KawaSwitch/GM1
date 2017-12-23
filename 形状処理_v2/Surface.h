#pragma once

#include "Object.h"
#include "ControlPoint.h"
#include "Curve.h"

extern Scene* test_scene;

// �ȖʃC���^�[�t�F�[�X
class Surface : public Object
{
protected:

    int _ordU, _ordV; // �K��
    int _nVertex; // ���_��

    int _ncpntU, _ncpntV; // ����_��
    vector<double> _ctrlpX; // �v�Z�p
    vector<double> _ctrlpY;
    vector<double> _ctrlpZ;

    GLuint _vbo_nor = 0; // �@���pvbo
    GLuint _ibo_nor = 0; // �@���pibo
    
    // �`��͈̓p�����[�^
    double _min_draw_param_U, _max_draw_param_U;
    double _min_draw_param_V, _max_draw_param_V;

    int _mesh_displayList = 0; // ���b�V���p�f�B�X�v���C���X�g
    double _mesh_width; // ���b�V�����̕�

    // ����_�ݒ�
    void SetControlPoint(ControlPoint* cp, int size);

    // �x�N�g���擾�֐�
    virtual Vector3d GetPositionVector(double u, double v) = 0; // �ʒu�x�N�g��
    virtual Vector3d GetFirstDiffVectorU(double u, double v) = 0; // �ڐ��x�N�g��
    virtual Vector3d GetFirstDiffVectorV(double u, double v) = 0;
    virtual Vector3d GetSecondDiffVectorUU(double u, double v) { return Vector3d(); }; // 2�K�����x�N�g��
    virtual Vector3d GetSecondDiffVectorUV(double u, double v) { return Vector3d(); };
    virtual Vector3d GetSecondDiffVectorVV(double u, double v) { return Vector3d(); };

    // �@���x�N�g���擾
    Vector3d GetNormalVector(double u, double v) { return (GetFirstDiffVectorU(u, v) * GetFirstDiffVectorV(u, v)); }

    // �`��p�ȗ��x�N�g���擾
    Vector3d GetCurvatureVector(double u, double v);

    // �w�肵���[�̋Ȑ��̐���_���擾����
    vector<ControlPoint> GetEdgeCurveControlPoint(SurfaceEdge edge);

    // ���b�V���\��
    virtual void DrawMeshInternal() { };
    void DrawMesh() { DrawUsingDisplayList(&_mesh_displayList, [&] { return (*this).DrawMeshInternal(); }); }

private:

    // �I�u�W�F�N�g�`��
    void Draw() override;

    // ����_�`��
    void DrawCPsInternal() override;

    // ��ȗ����擾
    void GetPrincipalCurvatures(double u, double v, double* max_kappa, double* min_kappa);
    // ���ϋȗ��擾
    double GetMeanCurvature(double u, double v);
    // �K�E�X�ȗ��擾
    double GetGaussianCurvature(double u, double v);

public:

    // �w�肵���p�����[�^�̃A�C�\�Ȑ����擾����
    //virtual Curve* GetIsoCurve(ParamUV direct, double param) = 0;

    // �w�肵���[�̋Ȑ����擾����
    virtual Curve* GetEdgeCurve(SurfaceEdge edge) = 0;

    // �Q�Ɠ_����̍ŋߓ_���擾
    Vector3d GetNearestPointFromRef(Vector3d ref);

    virtual ~Surface() { glDeleteLists(_mesh_displayList, 1); }
};