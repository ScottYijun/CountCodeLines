#ifndef CCOMMONHEADER_H
#define CCOMMONHEADER_H

#include <QString>

typedef struct FileInformation
{
    bool bChecked;
    QString fileName;       //文件名
    QString fileType;       //文件类型
    qint64 fileSize;        //文件大小
    qint32 totalLines;      //总行数
    quint32 codeLines;      //代码行数
    quint32 noteLines;      //注释行数
    quint32 blankLines;     //空白行数
    QString filePath;       //文件路径
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
