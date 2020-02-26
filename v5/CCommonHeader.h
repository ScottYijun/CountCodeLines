#ifndef CCOMMONHEADER_H
#define CCOMMONHEADER_H

#include <QString>

typedef struct FileInformation
{
    bool bChecked;
    QString fileName;       //�ļ���
    QString fileType;       //�ļ�����
    qint64 fileSize;        //�ļ���С
    qint32 totalLines;      //������
    quint32 codeLines;      //��������
    quint32 noteLines;      //ע������
    quint32 blankLines;     //�հ�����
    QString filePath;       //�ļ�·��
}FileInfor;


enum TableColumn
{
    FILE_CHECK_COLUMN = 0,
    FILE_NAME_COLUMN,
    FIlE_TYPE_COLOUMN,
    FILE_SIZE_COLUMN,
    TOTAL_LINES_COLUMN,
    CODE_LINES_COLUMN,
    NOTE_LINES_COLOUMN,
    BLANK_LINES_COLUMN,
    FILE_PATH_COLUMN
};

enum COUNTTYPE{
    EN_COUNT_DIR_CODE = 0,
    EN_COUNT_FILE_CODE
};

#endif // CCOMMONHEADER_H