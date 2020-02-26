#ifndef CTABLEMODEL_H
#define CTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "CCommonHeader.h"


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
    //��ʱˢ������
    void updateData();
    //�������
    void addData(const FileInfor &fileInfo);
    //�������
    void clearData();

private:
    QList<FileInfor> m_listFileInfo;

};

#endif // CTABLEMODEL_H


//https://yq.aliyun.com/articles/24427
