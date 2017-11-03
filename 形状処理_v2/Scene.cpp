#include "Scene.h"
#include "Axis.h"

Scene::Scene()
{

}

void Scene::AddObject(Object* obj)
{
    ObjList.push_back(obj);
}

void Scene::DeleteObjectAll()
{
    auto it = ObjList.begin();

    while (it != ObjList.end())
    {
        (*it)->RaiseDeleteFlag();

        it++;
    }
}

void Scene::DeleteObjectEnd()
{
    auto it = ObjList.begin();

    if (it == ObjList.end())
        return;

    while (it != ObjList.end())
        it++;

    (*(--it))->RaiseDeleteFlag();
}

void Scene::ToggleDrawControlPoints()
{
    auto it = ObjList.begin();

    // ����_�\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawCtrlp();
        it++;
    }
}

void Scene::ToggleDrawFirstDiffVectors()
{
    auto it = ObjList.begin();

    // �ڐ��\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawFisrtDiff();
        it++;
    }
}

void Scene::ToggleDrawSecondDiffVectors()
{
    auto it = ObjList.begin();

    // 2�K�����x�N�g���\���g�O��
    while (it != ObjList.end())
    {
        (*it)->SetUnsetIsDrawSecondDiff();
        it++;
    }
}

void Scene::Draw()
{
    auto it = ObjList.begin();

    // ���X�g�S�`��
    while (it != ObjList.end())
    {
        (*it)->Draw(); // ���f���`��
        (*it)->DrawControlPointsAndLines(); // ����_�`��
        (*it)->DrawFirstDiffVectors(); // �ڐ��`��
        (*it)->DrawSecondDiffVectors(); // 2�K�����x�N�g���`��
        (*it)->DrawBox(); // �~�j�}�N�X�{�b�N�X�`��
        (*it)->DrawCurvatureVectors(); // �ȗ��x�N�g���`��

        // �폜�t���O�`�F�b�N
        if ((*it)->IsDeleteFlagRaised())
        {
            delete *it;
            (*it) = NULL;
        }

        it++;
    }

    ObjList.remove(NULL);
}

// �}�E�X�s�b�L���O�p�`��
void Scene::DrawForPick()
{
    auto it = ObjList.begin();

    // ���X�g�S�`��
    while (it != ObjList.end())
    {
        glLoadName((*it)->GetObjectNumber());
        (*it)->DrawAsItIs();

        it++;
    }
}

Scene::~Scene()
{
    for (auto it = ObjList.begin(); it != ObjList.end(); it++)
        delete *it;
}