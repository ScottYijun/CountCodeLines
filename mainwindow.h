#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfoList>
#include <QPixmap>
#include <vector>
#include <string>
#include <list>
#include <map>

using std::vector;
using std::string;
using std::map;
using std::multimap;
using std::list;


typedef struct FileInformation
{
    QString fileName;   //文件名
    qint64 fileSize;    //文件大小
    quint32 fileLine;   //代码行数
    QString fileDir;    //文件所在目录
}FileInfor;


namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QFileInfoList getFileList(QString path);
    void initView();
    void initTableWidget();
    void initFileInformation();

    void vectorSum();
    void vectorAccumulate();
    void vectorIterator();


public slots:
    void slotOpenDir();                     //打开目录
    void slotOpenFile();                    //打开文件
    void slotVectorFilenameSizeLines();     //显示文件名，大小，行数, 路径
    void slotListFilenameSizeLines();       //显示文件名，大小，行数, 路径
    void slotMapFilenameSize();             //显示文件名，大小
    void slotMultimFilenameSize();          //显示文件名，大小

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    QString floder;
    int totalLines;
    QPixmap m_pixmap;
    vector<FileInfor> vecFileInfor;
    list<FileInfor> listFileInfor;
    vector<qint64> vecFileSize;
    map<QString, quint64> mapFileInfor;
    multimap<QString, quint64> multimapFileInfor;
};

#endif // MAINWINDOW_H
