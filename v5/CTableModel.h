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
    //定时刷新数据
    void updateData();
    //添加数据
    void addData(const FileInfor &fileInfo);
    //清除数据
    void clearData();

private:
    QList<FileInfor> m_listFileInfo;

};

#endif // CTABLEMODEL_H


//https://yq.aliyun.com/articles/24427
