#pragma once

#include "GV.h"
#include <string>
#include <sstream>
#include "Object.h"

// �t�@�C���ǂݍ��݃N���X
class Reader
{
protected:

    string _path; // �f�B���N�g���p�X

public:

    vector<string> GetFilenames(string extension = "");
    vector<string> ReadAllLines(string filepath);

    Reader() { _path = ""; }
    Reader(string path) { _path = path; }
    ~Reader() {};
};

// �`�󏈗� �Ȑ�/�Ȗʓǂݍ��ݗp�N���X
class KjsReader : public Reader
{
private:

    string KJS_FILE_NAME = "KJS_FILE"; // .kjs�t�@�C���̊i�[�t�H���_��

    Object* BezierCurveReader(vector<string> lines); // �x�W�G�Ȑ�
    Object* BezierSurfaceReader(vector<string> lines); // �x�W�G�Ȗ�
    Object* BsplineCurveReader(vector<string> lines); // B�X�v���C���Ȑ�
    Object* BsplineSurfaceReader(vector<string> lines); // B�X�v���C���Ȗ�

public:

    vector<Object *> GetObjectsFromKjsFolder();
    Object* GetObjectFromFile(string file_name);

    using Reader::Reader;
};
