#ifndef CWORKTHREAD_H
#define CWORKTHREAD_H

#include <QThread>
#include <QList>
#include "CCommonHeader.h"
#include "CTableModel.h"

class CWorkThread : public QThread
{
    Q_OBJECT

public:
    CWorkThread(CTableModel *pTableModel);
    void run();
    bool checkFile(const QString &fileName);
    void filterFile(const QString &filePath);//�����ļ�
    void countFile(const QStringList &files);//ͳ���ļ�
    void countCode(const QString &fileName, int &lineCode, int &lineBlank, int &lineNotes);//ͳ�Ƶ����ļ��Ĵ���
    void setCountDirParam(const QString path, COUNTTYPE type, const QString filterType);//����ͳ��Ŀ¼����Ĳ���
    void setCountFileParam(const QStringList listFile, COUNTTYPE type, const QString filterType);//����ͳ���ļ�����Ĳ���

signals:
    void signalUpdateData(int nFileCount, quint32 totalBytes, quint32 totalLines, quint32 totalCodes, quint32 totalNotes, quint32 totalBlank);

private:
    QStringList m_listFile;
    QList<FileInfor> m_listModel;
    QString m_strFilterType;
    CTableModel *m_pTableModel;
    QString m_strPath;
    COUNTTYPE m_EnType;
};

#endif // CWORKTHREAD_H
