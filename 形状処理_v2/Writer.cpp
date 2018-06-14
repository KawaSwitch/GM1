#include "Writer.h"
#include <fstream>
#include <ctime>

// �^�C���X�^���v�������o��
void ExportTimeStamp(std::ofstream* log)
{
    char buf[256];
    int buf_size = sizeof(buf);

    time_t longtime = time(NULL);
    struct tm now;
    errno_t error;
    char week[][3] = { "��","��","��","��","��","��","�y" };

    error = localtime_s(&now, &longtime);
    asctime_s(buf, buf_size, &now);

    *log << buf << std::endl;
}

// �Ȑ��̍ŋߓ_����CSV�ɏ����o��
void ExportCsv_NearestInfoCurve(std::string path, std::vector<NearestPointInfoC> np_infos, bool isNeedTimestamp)
{
    char buf[256];
    int buf_size = sizeof(buf);

    std::ofstream log;
    log.open(path, std::ios::trunc); // ���t�@�C����������Ώ㏑��

    std::string value = ", �ŋߓ_X���W, �ŋߓ_Y���W, �ŋߓ_Z���W, �p�����[�^, �_�ԋ���";
    log << value << std::endl;
    
    int cnt = 0;
    for (const auto& info : np_infos)
    {
        std::string np_data;

        // �C���f�b�N�X
        sprintf_s(buf, buf_size, "%d,", ++cnt);
        np_data += buf;

        // ���W
        sprintf_s(buf, buf_size, "%f,", info.nearestPnt.X);
        np_data += buf;
        sprintf_s(buf, buf_size, "%f,", info.nearestPnt.Y);
        np_data += buf;
        sprintf_s(buf, buf_size, "%f,", info.nearestPnt.Z);
        np_data += buf;

        // �p�����[�^
        sprintf_s(buf, buf_size, "%f,", info.param);
        np_data += buf;

        // �_�ԋ���
        sprintf_s(buf, buf_size, "%f", info.dist);
        np_data += buf;

        log << np_data << std::endl;
    }

    if (isNeedTimestamp)
        ExportTimeStamp(&log);

    log.close();
}

// �Ȗʂ̍ŋߓ_����CSV�ɏ����o��
void ExportCsv_NearestInfoSurface(std::string path, std::vector<NearestPointInfoS> np_infos, bool isNeedTimestamp)
{
    char buf[256];
    int buf_size = sizeof(buf);

    std::ofstream log;
    log.open(path, std::ios::trunc); // ���t�@�C����������Ώ㏑��

    std::string value = ", �ŋߓ_X���W, �ŋߓ_Y���W, �ŋߓ_Z���W, U�p�����[�^, V�p�����[�^, �_�ԋ���";
    log << value << std::endl;

    int cnt = 0;
    for (const auto& info : np_infos)
    {
        std::string np_data;

        // �C���f�b�N�X
        sprintf_s(buf, buf_size, "%d,", ++cnt);
        np_data += buf;

        // ���W
        sprintf_s(buf, buf_size, "%f,", info.nearestPnt.X);
        np_data += buf;
        sprintf_s(buf, buf_size, "%f,", info.nearestPnt.Y);
        np_data += buf;
        sprintf_s(buf, buf_size, "%f,", info.nearestPnt.Z);
        np_data += buf;

        // �p�����[�^
        sprintf_s(buf, buf_size, "%f,", info.paramU);
        np_data += buf;
        sprintf_s(buf, buf_size, "%f,", info.paramV);
        np_data += buf;

        // �_�ԋ���
        sprintf_s(buf, buf_size, "%f", info.dist);
        np_data += buf;

        log << np_data << std::endl;
    }

    if (isNeedTimestamp)
        ExportTimeStamp(&log);

    log.close();
}