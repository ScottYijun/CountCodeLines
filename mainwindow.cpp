#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <numeric>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    floder = "";
    m_pixmap.load(":/background.jpg");
    m_pixmap = m_pixmap.scaled(800, 600);
    this->setToolTip("代码行统计工具");
	
    initTableWidget();
    initView();
    connect(ui->pushButton_openFloder, SIGNAL(clicked()), this, SLOT(slotOpenDir()));
    connect(ui->pushButton_vector, SIGNAL(clicked()), this, SLOT(slotVectorFilenameSizeLines()));
    connect(ui->pushButton_list, SIGNAL(clicked()), this, SLOT(slotListFilenameSizeLines()));
    connect(ui->pushButton_map, SIGNAL(clicked()), this, SLOT(slotMapFilenameSize()));
    connect(ui->pushButton_multimap, SIGNAL(clicked()), this, SLOT(slotMapFilenameSize()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initView()
{
    //ui->pushButton_openFloder->setFlat( true );
    ui->pushButton_openFloder->setStyleSheet( "QPushButton{background: transparent;}\
            QPushButton{border-color: #ff0000;border-width: 1px; border-style:solid;}\
            QPushButton:hover{border-color: #00ff00;border-width: 1px; border-style:solid;}\
            QPushButton:pressed{border-color: #0000ff;border-width: 1px; border-style:solid;}");
    ui->pushButton_vector->setStyleSheet( "QPushButton{background: transparent;}\
            QPushButton{border-color: #ff0000;border-width: 1px; border-style:solid;}\
            QPushButton:hover{border-color: #00ff00;border-width: 1px; border-style:solid;}\
            QPushButton:pressed{border-color: #0000ff;border-width: 1px; border-style:solid;}");
    ui->pushButton_list->setStyleSheet( "QPushButton{background: transparent;}\
            QPushButton{border-color: #ff0000;border-width: 1px; border-style:solid;}\
            QPushButton:hover{border-color: #00ff00;border-width: 1px; border-style:solid;}\
            QPushButton:pressed{border-color: #0000ff;border-width: 1px; border-style:solid;}");
    ui->pushButton_map->setStyleSheet( "QPushButton{background: transparent;}\
            QPushButton{border-color: #ff0000;border-width: 1px; border-style:solid;}\
            QPushButton:hover{border-color: #00ff00;border-width: 1px; border-style:solid;}\
            QPushButton:pressed{border-color: #0000ff;border-width: 1px; border-style:solid;}");
    ui->pushButton_multimap->setStyleSheet( "QPushButton{background: transparent;}\
            QPushButton{border-color: #ff0000;border-width: 1px; border-style:solid;}\
            QPushButton:hover{border-color: #00ff00;border-width: 1px; border-style:solid;}\
            QPushButton:pressed{border-color: #0000ff;border-width: 1px; border-style:solid;}");
    ui->lineEdit->setStyleSheet( "QLineEdit{background: transparent;}" );
    ui->tableWidget->setStyleSheet( "QTableWidget{background: transparent;}" );
}

void MainWindow::initTableWidget()
{
    ui->tableWidget->clear();
    int count = ui->tableWidget->rowCount();
    for(int i = 0; i < count; ++i)
    {
        ui->tableWidget->removeRow(i);
    }
}

void MainWindow::initFileInformation()
{
    //wc -l `find . -name '*.h' -or -name '*.cpp'`
    QFileInfoList file_list = getFileList(ui->lineEdit->text());
    qDebug() << "file_list====================" << file_list.size();
    totalLines = 0;
    vecFileInfor.clear();
    mapFileInfor.clear();
    multimapFileInfor.clear();
    listFileInfor.clear();
    vecFileSize.clear();
    for (int i = 0; i < file_list.size(); ++i)
    {
        QFileInfo fileInfo = file_list.at(i);
        //qDebug() << "file" << i << "=====" << fileInfo.fileName() << "suffix==" << fileInfo.suffix() << fileInfo.filePath();

        QFile file(fileInfo.filePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        FileInfor tempFileInfo;
        quint32 currentFileLine = 0;
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();//每次读取一行
            int nLine = line.size();
            if(nLine > 1)
            {
                ++totalLines;
                ++currentFileLine;
            }
        }
        tempFileInfo.fileName = fileInfo.fileName();
        tempFileInfo.fileLine = currentFileLine;
        tempFileInfo.fileDir = fileInfo.absolutePath();
        tempFileInfo.fileSize = file.size();
        listFileInfor.push_back(tempFileInfo);
        vecFileInfor.push_back(tempFileInfo);
        vecFileSize.push_back(tempFileInfo.fileSize);
        mapFileInfor.insert({fileInfo.fileName(), file.size()});
        multimapFileInfor.insert({fileInfo.fileName(), file.size()});
        qDebug() << "vecFileInfor.size()=====================" << vecFileInfor.size();

    }
    qDebug() << "totalLines=====================" << totalLines;
    //Qt实现遍历文件夹和文件目录，并过滤和获取文件信息、后缀名、前缀名
    //http://blog.csdn.net/bzhxuexi/article/details/10095651
    ui->label->setText(QString("%1 行").arg(totalLines));
}

void MainWindow::slotOpenDir()
{
    //打开目录
    floder = QFileDialog::getExistingDirectory(this, tr("Open Dir"));
    ui->lineEdit->setText(floder);
    initFileInformation();
}

void MainWindow::slotOpenFile()
{
    //floder = QFileDialog::getOpenFileName(this, tr("Open Folder"), "", tr("Allfile(*.*)"));
    //打开文件
    floder = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.h *.cpp)"));
    int location = floder.lastIndexOf("/");
    QString dir = floder.left(location);
    qDebug() << "location====" << location << "dir====================" << dir;
    QString x = "crazy azimuths";
    QString y = "az";
    qDebug() << "index1==" << x.lastIndexOf(y);           // returns 6
    qDebug() << "index2==" << x.lastIndexOf(y, 6);        // returns 6
    qDebug() << "index3==" << x.lastIndexOf(y, 5);        // returns 2
    qDebug() << "index4==" << x.lastIndexOf(y, 1);
}
//http://www.cnblogs.com/findumars/p/6006129.html
QFileInfoList MainWindow::getFileList(QString path)
{
    QDir dir(path);
    //QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    //读取.h, .cpp文件
    QStringList strings;
    strings << "*.h" << "*.cpp" ;
    QFileInfoList file_list = dir.entryInfoList(strings);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath();
        qDebug() << "name====================" << floder;
        QFileInfoList child_file_list = getFileList(name);
        file_list.append(child_file_list);
    }

    return file_list;
}

//http://www.cnblogs.com/zhoug2020/p/3789076.html
void MainWindow::slotVectorFilenameSizeLines()
{
    initTableWidget();
    ui->tableWidget->setColumnCount(4);
    QStringList tableHeader;
    tableHeader << tr("fileName") << tr("size") << tr("line") << tr("path");
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    initFileInformation();

//    for(int i = 0; i < tempFileInforSize; ++i)
//    {
//        FileInfor tempFileInfo = vecFileInfor.at(i);
//        ui->tableWidget->insertRow(i);
//        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(tempFileInfo.fileName));
//        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(tempFileInfo.fileSize)));
//        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(tempFileInfo.fileLine)));
//        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(iter.fileDir)));
//    }

    //c++ 11
    int i = 0;
    for(auto iter: vecFileInfor)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(iter.fileName));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(iter.fileSize)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(iter.fileLine)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(iter.fileDir));
        ++i;
    }
}


void MainWindow::slotListFilenameSizeLines()
{
    initTableWidget();
    ui->tableWidget->setColumnCount(4);
    QStringList tableHeader;
    tableHeader << tr("fileName") << tr("size") << tr("line") << tr("path");
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    initFileInformation();

//    for(int i = 0; i < tempFileInforSize; ++i)
//    {
//        FileInfor tempFileInfo = listFileInfor.at(i);
//        ui->tableWidget->insertRow(i);
//        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(tempFileInfo.fileName));
//        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(tempFileInfo.fileSize)));
//        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(tempFileInfo.fileLine)));
//        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(iter.fileDir)));
//    }

    //c++ 11
    int i = 0;
    for(auto iter: listFileInfor)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(iter.fileName));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(iter.fileSize)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(iter.fileLine)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(iter.fileDir));
        ++i;
    }
}

void MainWindow::slotMapFilenameSize()
{
    initTableWidget();
    ui->tableWidget->setColumnCount(2);
    QStringList tableHeader;
    tableHeader << tr("fileName") << tr("size");
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    initFileInformation();

    int i = 0;
//    map<QString, quint64>::iterator iter = mapFileInfor.begin();
//    for(; iter != mapFileInfor.end(); ++iter)
//    {
//        ui->tableWidget->insertRow(i);
//        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(iter->first));
//        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(iter->second)));
//    }

    //c++ 11
    for(auto iter: mapFileInfor)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(iter.first));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(iter.second)));
    }
}

void MainWindow::slotMultimFilenameSize()
{
    initTableWidget();
    ui->tableWidget->setColumnCount(2);
    QStringList tableHeader;
    tableHeader << tr("fileName") << tr("size");
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    initFileInformation();

    int i = 0;
//    multimap<QString, quint64>::iterator iter = multimapFileInfor.begin();
//    for(; iter != multimapFileInfor.end(); ++iter)
//    {
//        ui->tableWidget->insertRow(i);
//        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(iter->first));
//        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(iter->second)));
//    }

    //c++ 11
    for(auto iter: multimapFileInfor)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(iter.first));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(iter.second)));
    }
}

void MainWindow::vectorSum()
{
    quint64 totalSize = 0;
    int count = vecFileSize.size();
    for(int i = 0; i < count; ++i)
    {
        totalSize += vecFileSize.at(i);
    }
    qDebug() << "vectorSum========================" << totalSize;
}

void MainWindow::vectorAccumulate()
{
    qint64 totalSize = 0;
    totalSize = accumulate(vecFileSize.cbegin(), vecFileSize.cend(), 0);
    qDebug() << "vectorAccumulate========================" << totalSize;
}

void MainWindow::vectorIterator()
{
    qint64 totalSize = 0;
    //auto begin = vecFileSize.begin(), end = vecFileSize.end();
    vector<qint64>::iterator begin = vecFileSize.begin(), end = vecFileSize.end();
    while(begin != end)
    {
        totalSize += *begin;
        ++begin;
    }
    qDebug() << "vectorIterator========================" << totalSize;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(QRect(0, 0, this->width(), this->height()), m_pixmap);
    //painter.drawText()
}

