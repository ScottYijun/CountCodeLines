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
    QString fileName;       //文件名
    QString fileType;       //文件类型
    qint64 fileSize;        //文件大小
    qint32 totalLines;      //总行数
    quint32 codeLines;      //代码行数
    quint32 commentLines;   //注释行数
    quint32 emptyLines;     //空白行数
    QString filePath;       //文件路径
}FileInfor;


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
    void initTableWidget();
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
    QString floder;
    int totalLines;
    QPixmap m_pixmap;
    QStringList m_listFile;
};

#endif // MAINWINDOW_H
