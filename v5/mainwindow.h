#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfoList>
#include <QPixmap>
#include <QStandardItemModel>
#include "CTableModel.h"
#include "CWorkThread.h"

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QFileInfoList getFileList(QString path);
    void initView();
    void initTableView();
    void initData();
    void initConnect();


public slots:
    void slotOpenDir();                     //打开目录
    void slotOpenFile();                    //打开文件
    void slotClearResult();                 //清理结果;
    void slotLabelLink(const QString &link);
    void slotUpdateData(int nFileCount, quint32 totalBytes, quint32 totalLines, quint32 totalCodes, quint32 totalNotes, quint32 totalBlanks);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    CWorkThread *m_pWorkThread;
    QPixmap m_pixmap;
    QStringList m_listFile;
    QList<FileInfor> m_listModel;
    CTableModel *m_pTableModel;
};

#endif // MAINWINDOW_H
