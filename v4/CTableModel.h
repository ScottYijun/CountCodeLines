#ifndef CTABLEMODEL_H
#define CTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

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

class CTableModel:public QAbstractTableModel
{
    Q_OBJECT

public:
    CTableModel();
    //����
    int rowCount(const QModelIndex &parent) const;
    //����
    int columnCount(const QModelIndex &parent) const;
    //���ñ����Ŀ����
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    //�����Ŀ����
    QVariant data(const QModelIndex &index, int role) const;
    //��ͷ����
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //����ѡ�У��ɸ�ѡ��־
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //��������
    void updateData(QList<FileInfor> &listFile);

    QList<FileInfor> m_listFileInfo;

};

#endif // CTABLEMODEL_H


//https://yq.aliyun.com/articles/24427
