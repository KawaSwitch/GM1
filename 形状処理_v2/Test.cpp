#include "GV.h"
#include "Test.h"
#include "Model.h"
#include "Reader.h"
#include "Scene.h"

static int current_size; // ���݂̃e�X�g�p�̃f�B�X�v���C���X�g��
static int* displayLists; // �f�B�X�v���C���X�g�ۊǗp�|�C���^
static bool isFirst = true;

Scene* test_scene;

// �`�悵�����̂�����
static vector<function<void(void)>> TestRegisterDraw
{
    //DrawBsplineFunctions, // B�X�v���C�����֐��`��
    //DrawBsplineCurves, // B�X�v���C���Ȑ��`��
    //TestGetNearestPointCurveToCurve_CGS04, // �Ȑ��ƋȐ��̍ŋߓ_�Q�`��
    //TestGetNearestPointCurveToSurface_CGS04, // �Ȑ��ƋȖʂ̍ŋߓ_�Q�`��
    //ShowLUDecomp, // LU�����̌��ʂ��m���߂�
    TestDrawCircleWithNurbsCurve_CGS3, // Nurbs�Ȑ��ŉ~�`��
};

// Nurbs�Ȑ��ŉ~��`��
void TestDrawCircleWithNurbsCurve_CGS3()
{
    ControlPoint cp0[3]
    {
        ControlPoint(5, 0, 0, 1.0),
        ControlPoint(5, 5, 0, 1 / sqrt(2)),
        ControlPoint(0, 5, 0, 1.0),
    };
    ControlPoint cp1[3]
    {
        ControlPoint(0, 5, 0, 1.0),
        ControlPoint(-5, 5, 0, 1 / sqrt(2)),
        ControlPoint(-5, 0, 0, 1.0),
    };
    ControlPoint cp2[3]
    {
        ControlPoint(-5, 0, 0, 1.0),
        ControlPoint(-5, -5, 0, 1 / sqrt(2)),
        ControlPoint(0, -5, 0, 1.0),
    };
    ControlPoint cp3[3]
    {
        ControlPoint(0, -5, 0, 1.0),
        ControlPoint(5, -5, 0, 1 / sqrt(2)),
        ControlPoint(5, 0, 0, 1.0),
    };

    double knot[6] = { 0, 0, 0, 1, 1, 1 };

    NurbsCurve* curve0 = new NurbsCurve(3, cp0, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve1 = new NurbsCurve(3, cp1, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve2 = new NurbsCurve(3, cp2, 3, knot, Color::blue, 2.0);
    NurbsCurve* curve3 = new NurbsCurve(3, cp3, 3, knot, Color::blue, 2.0);

    if (isFirst)
    {
        test_scene->AddObject(curve0);
        test_scene->AddObject(curve1);
        test_scene->AddObject(curve2);
        test_scene->AddObject(curve3);
    }
}

// LU�����̌��ʂ�\������
void ShowLUDecomp()
{
    double A[2][2] = { { 2, 1 }, { 3, 1 } };
    double B[2] = { 5, 6 };

    auto x = LUDecomposition<2>(A, B);
    printf("%f %f\n", x[0], x[1]);
}

// �Q�ƋȐ���ɗ��U�_�𐶐���, ������O���_�Ƃ��čŋߓ_�����߂�
// ���W����04��_05��A.docx
void TestGetNearestPointCurveToCurve_CGS04()
{
    auto reader = new KjsReader("KJS_FILE/");

    // �ΏۋȐ�/�Ȗ�
    Curve* curve1 = (Curve *)reader->GetObjectFromFile("CGS_bspline_curve_1.kjs");
    // �Q�ƋȐ�
    Curve* curveC = (Curve *)reader->GetObjectFromFile("CGS_bspline_curve_C.kjs");

    // �Q�Ɠ_�Q���擾
    auto ref_pnts = curveC->GetPositionVectors(20);
    vector<Vector3d> nearest_pnts; // �ŋߓ_�Q

    // �ŋߓ_�擾
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(curve1->GetNearestPointFromRef(ref_pnts[i]));

    // �`��
    glLineWidth(1.0);
    glPointSize(5.0);
    glBegin(GL_LINES);
    glColor3dv(Color::orange);
    for (int i = 0; i < (int)ref_pnts.size(); i++)
    {
        glVertex3d(ref_pnts[i]);
        glVertex3d(nearest_pnts[i]);
    }
    glEnd();

    glBegin(GL_POINTS);
    glColor3dv(Color::red);
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        glVertex3d(ref_pnts[i]);

    glColor3dv(Color::light_green);
    for (int i = 0; i < (int)nearest_pnts.size(); i++)
        glVertex3d(nearest_pnts[i]);
    glEnd();

    if (isFirst)
    {
        test_scene->AddObject(curve1);
        test_scene->AddObject(curveC);
    }
}
void TestGetNearestPointCurveToSurface_CGS04()
{
    auto reader = new KjsReader("KJS_FILE/");

    // �ΏۋȐ�/�Ȗ�
    Surface* surf1 = (Surface *)reader->GetObjectFromFile("CGS_bspline_surface_1.kjs");
    // �Q�ƋȐ�
    Curve* curveS = (Curve *)reader->GetObjectFromFile("CGS_bspline_curve_S.kjs");

    // �Q�Ɠ_�Q���擾
    auto ref_pnts = curveS->GetPositionVectors(20);
    vector<Vector3d> nearest_pnts; // �ŋߓ_�Q

    // �ŋߓ_�擾
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        nearest_pnts.push_back(surf1->GetNearestPointFromRef(ref_pnts[i]));

    // �`��
    glLineWidth(1.0);
    glPointSize(5.0);
    glBegin(GL_LINES);
    glColor3dv(Color::orange);
    for (int i = 0; i < (int)ref_pnts.size(); i++)
    {
        glVertex3d(ref_pnts[i]);
        glVertex3d(nearest_pnts[i]);
    }
    glEnd();

    glBegin(GL_POINTS);
    glColor3dv(Color::red);
    for (int i = 0; i < (int)ref_pnts.size(); i++)
        glVertex3d(ref_pnts[i]);

    glColor3dv(Color::light_green);
    for (int i = 0; i < (int)nearest_pnts.size(); i++)
        glVertex3d(nearest_pnts[i]);
    glEnd();

    if (isFirst)
    {
        test_scene->AddObject(surf1);
        test_scene->AddObject(curveS);
    }
}

// ���֐�����т��̊֐����g����B�X�v���C���Ȑ���`�悷��
// ��1��ۑ�
static double knot_a[10] = { -3, -2, -1, 0, 1, 2, 3, 4, 5, 6 };
static double knot_b[10] = { 0, 0, 0, 0, 1, 2, 3, 3, 3, 3 };
static double knot_c[10] = { 0, 0, 0, 0, 1, 1, 3, 3, 3, 3 };
void DrawBsplineFunctions()
{
    // �ʂ̂Ƃ��Ńf�B�X�v���C���X�g��static�Ŏg���Ă�̂œ����ɂ͏����Ȃ�
    //DrawBsplineFunc(4, 6, 10, knot_a, -3.0, 6.0);
    DrawBsplineFunc(4, 6, 10, knot_b, 0.0, 3.0);
    //DrawBsplineFunc(4, 6, 10, knot_c, 0.0, 3.0);
}
void DrawBsplineCurves()
{
    static ControlPoint cp[6] =
    {
        ControlPoint(30, 80, 0),
        ControlPoint(50, 110, 0),
        ControlPoint(70, 120, 0),
        ControlPoint(90, 70, 0),
        ControlPoint(110, 60, 0),
        ControlPoint(130, 80, 0),
    };

    auto A = new BsplineCurve(4, cp, 6, knot_a, Color::blue, 1.0);
    auto B = new BsplineCurve(4, cp, 6, knot_b, Color::orange, 1.0);
    auto C = new BsplineCurve(4, cp, 6, knot_c, Color::green, 1.0);

    // ����_���݂ŕ`��
    A->DrawAsItIsWithCPs();
    B->DrawAsItIsWithCPs();
    C->DrawAsItIsWithCPs();
}

// �e�X�g�`�惁�C��
void TestDraw()
{
    // ����̓f�B�X�v���C���X�g�쐬
    if (isFirst)
    {
        for (int i = 0; i < (int)TestRegisterDraw.size(); i++)
        {
            displayLists = (int *)realloc(displayLists, sizeof(int) * ++current_size);
            displayLists[i] = glGenLists(1);

            glNewList(displayLists[i], GL_COMPILE);
            TestRegisterDraw[i]();
            glEndList();
        }

        isFirst = false;
        glutPostRedisplay();
    }
    // ����ȊO�̓R�[��
    else
    {
        for (int i = 0; i < (int)TestRegisterDraw.size(); i++)
        {
            if (displayLists[i])
                glCallList(displayLists[i]);
        }
    }

    // �e�X�g�V�[���`��
    test_scene->Draw();
}