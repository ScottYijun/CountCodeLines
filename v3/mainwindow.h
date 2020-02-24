#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfoList>
#include <QPixmap>
#include <QStandardItemModel>
#include "CTableModel.h"

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
    bool checkFile(const QString &fileName);

    void countCode(const QString &filePath);
    void countCode(const QStringList &files);
    void countCode(const QString &fileName, int &lineCode, int &lineBlank, int &lineNotes);


public slots:
    void slotOpenDir();                     //打开目录
    void slotOpenFile();                    //打开文件
    void slotClearResult();                 //清理结果

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    int totalLines;
    QPixmap m_pixmap;
    QStringList m_listFile;
    QList<FileInfor> m_listModel;
    CTableModel *m_pTableModel;
};

#endif // MAINWINDOW_H
