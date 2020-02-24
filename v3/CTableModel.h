#ifndef CTABLEMODEL_H
#define CTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

typedef struct FileInformation
{
    //bool bChecked;
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
    FILE_NAME_COLUMN = 0,
    FIlE_TYPE_COLOUMN,
    FILE_SIZE_COLUMN,
    TOTAL_LINES_COLUMN,
    CODE_LINES_COLUMN,
    NOTE_LINES_COLOUMN,
    BLANK_LINES_COLUMN,
    FILE_PATH_COLUMN
};

class CTableModel:public QAbstractTableModel
{
public:
    CTableModel();
    //行数
    int rowCount(const QModelIndex &parent) const;
    //列数
    int columnCount(const QModelIndex &parent) const;
    //设置表格项目数据
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    //表格项目数据
    QVariant data(const QModelIndex &index, int role) const;
    //表头数据
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //表格可选中，可复选标志
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //更新数据
    void updateData(QList<FileInfor> &listFile);

    QList<FileInfor> m_listFileInfo;

};

#endif // CTABLEMODEL_H


//https://yq.aliyun.com/articles/24427
