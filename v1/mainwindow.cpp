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
    //this->setToolTip(tr("code line statistics tool"));
    this->setWindowTitle(tr("code line statistics tool"));

    initView();
    initTableWidget();

    connect(ui->pushButton_openDir, SIGNAL(clicked()), this, SLOT(slotOpenDir()));
    connect(ui->pushButton_openFile, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(ui->pushButton_clearResult, SIGNAL(clicked()), this, SLOT(slotClearResult()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initView()
{
    ui->pushButton_openFile->setFlat(true);
    ui->pushButton_openFile->setStyleSheet( "QPushButton{background: transparent;}\
                                              QPushButton{border-color: #ff0000;border-width: 1px; border-style:solid;}\
                                              QPushButton:hover{border-color: #00ff00;border-width: 1px; border-style:solid;}\
                                              QPushButton:pressed{border-color: #0000ff;border-width: 1px; border-style:solid;}");

    ui->pushButton_openDir->setStyleSheet( "QPushButton{background: transparent;}\
                                            QPushButton{border-color: #ff0000;border-width: 1px; border-style:solid;}\
                                            QPushButton:hover{border-color: #00ff00;border-width: 1px; border-style:solid;}\
                                            QPushButton:pressed{border-color: #0000ff;border-width: 1px; border-style:solid;}");

    ui->pushButton_clearResult->setStyleSheet( "QPushButton{background: transparent;}\
                                              QPushButton{border-color: #ff0000;border-width: 1px; border-style:solid;}\
                                              QPushButton:hover{border-color: #00ff00;border-width: 1px; border-style:solid;}\
                                              QPushButton:pressed{border-color: #0000ff;border-width: 1px; border-style:solid;}");

    //ui->lineEdit->setStyleSheet( "QLineEdit{background: transparent;}" );
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
    ui->tableWidget->setColumnCount(8);
    QStringList tableHeader;
    //文件名， 类型，大小，总行数，代码行数，注释行数，空白行数，路径
    tableHeader << tr("fileName") << tr("type") << tr("size") << tr("total lines") << "code lines" << tr("note lines") << tr("blank lines") << tr("path");
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidget->verticalHeader()->setHighlightSections(false);

    QList<int> columnWidth;
    columnWidth << 130 << 50 << 70 << 80 << 70 << 70 << 70 << 150;

    //设置前景色
    ui->lineEdit_fileCount->setStyleSheet("color:#17A086;");
    ui->lineEdit_byteCount->setStyleSheet("color:#CA5AA6;");
    ui->lineEdit_totalLines->setStyleSheet("color:#CD1B19;");
    ui->lineEdit_codeLines->setStyleSheet("color:#22A3A9;");
    ui->lineEdit_emptyLines->setStyleSheet("color:#D64D54;");
    ui->lineEdit_commentLines->setStyleSheet("color:#A279C5;");

    //设置字体加粗
    QFont font;
    font.setBold(true);
    if (font.pointSize() > 0) {
        font.setPointSize(font.pointSize() + 1);
    } else {
        font.setPixelSize(font.pixelSize() + 2);
    }

    ui->lineEdit_fileCount->setFont(font);
    ui->lineEdit_byteCount->setFont(font);
    ui->lineEdit_totalLines->setFont(font);
    ui->lineEdit_codeLines->setFont(font);
    ui->lineEdit_emptyLines->setFont(font);
    ui->lineEdit_commentLines->setFont(font);

}

//检查文件
bool MainWindow::checkFile(const QString &fileName)
{
    if(fileName.startsWith("moc_") || fileName.startsWith("ui_") || fileName.startsWith("qrc_"))
    {
        return false;
    }

    QFileInfo file(fileName);//获取文件信息
    QString suffix = "*." + file.suffix();//文件后缀
    QString filter = ui->lineEdit_filter->text().trimmed();//去掉前面空格
    QStringList filters = filter.split(" ");
    return filters.contains(suffix);
}

void MainWindow::slotOpenDir()
{
    QString path =  QFileDialog::getExistingDirectory(this, tr("choose dir"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!path.isEmpty())
    {
        ui->lineEdit_dir->setText(path);
        m_listFile.clear();
        countCode(path);
        qDebug() << "m_listFile======" << m_listFile;
        countCode(m_listFile);
    }
}

void MainWindow::slotOpenFile()
{
    QString filter = QString(tr("code file(%1)").arg(ui->lineEdit_filter->text().trimmed()));
    QStringList files = QFileDialog::getOpenFileNames(this, tr("choose file"), "./", filter);
    if(files.size() > 0)
    {
        ui->lineEdit_fileName->setText(files.join("|"));
        countCode(files);
    }
}

void MainWindow::slotClearResult()
{
    ui->lineEdit_fileCount->setText("0");//文件数
    ui->lineEdit_byteCount->setText("0");//文件总大小
    ui->lineEdit_totalLines->setText("0");//文件总行数
    ui->lineEdit_codeLines->setText("0");//代码行数
    ui->lineEdit_commentLines->setText("0");//注释行数
    ui->lineEdit_emptyLines->setText("0");//空白行数

    ui->label_percentCode->setText("0%");
    ui->label_percentNotes->setText("0%");
    ui->label_percentBlank->setText("0%");
}

//http://www.cnblogs.com/findumars/p/6006129.html
//http://www.cnblogs.com/zhoug2020/p/3789076.html




//找出目录下所有符合条件的文件
void MainWindow::countCode(const QString &filePath)
{
    qDebug() << "filePath========================" << filePath;
    QDir dir(filePath);
    foreach (QFileInfo fileInfo, dir.entryInfoList()) //entryInfoList()返回QFileInfoList类型的对象
    {
        if(fileInfo.isFile())
        {
            QString strFileName = fileInfo.fileName();
            //qDebug() << "strFileName========================" << strFileName;
            if(checkFile(strFileName)) //检测符合条件的文件
            {
                qDebug() << "fileInfo.filePath========================" << fileInfo.filePath();
                m_listFile << fileInfo.filePath();
            }
        }
        else
        {
            if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            {
                continue;
            }
            //递归找出文件
            countCode(fileInfo.absoluteFilePath());
        }
    }

}

void MainWindow::countCode(const QStringList &files)
{
    int lineCode;   //代码行
    int lineBlank;  //空行数
    int lineNotes;  //注释行数
    int nCount = files.count();
    qDebug() << "nCount=====================" << nCount;
    ui->tableWidget->setRowCount(nCount);

    quint32 totalLines = 0;
    quint32 totalBytes = 0;
    quint32 totalCodes = 0;
    quint32 totalNotes = 0;
    quint32 totalBlanks = 0;

    for(int i = 0; i < nCount; ++i)
    {
        QFileInfo fileInfo(files.at(i));
        qDebug() << "files.at(" << i << ")===" << files.at(i);
        qDebug() << "fileInfo.filePath===" << fileInfo.filePath();
        countCode(fileInfo.filePath(), lineCode, lineBlank, lineNotes);

        int lineAll = lineCode + lineBlank + lineNotes;
        //文件名
        QTableWidgetItem *itemName = new QTableWidgetItem;
        itemName->setText(fileInfo.fileName());
        //文件类型
        QTableWidgetItem *itemSuffix = new QTableWidgetItem;
        itemSuffix->setText(fileInfo.suffix());
        //文件大小
        QTableWidgetItem *itemSize = new QTableWidgetItem;
        itemSize->setText(QString::number(fileInfo.size()));
        //代码总行数
        QTableWidgetItem *itemAllLine = new QTableWidgetItem;
        itemAllLine->setText(QString::number(lineAll));
        //代码行数
        QTableWidgetItem *itemLine = new QTableWidgetItem;
        itemLine->setText(QString::number(lineCode));

        //注释行数
        QTableWidgetItem *itemCommentLine = new QTableWidgetItem;
        itemCommentLine->setText(QString::number(lineNotes));
        //空行
        QTableWidgetItem *itemBlank = new QTableWidgetItem;
        itemBlank->setText(QString::number(lineBlank));
        //文件路径
        QTableWidgetItem *itemPath = new QTableWidgetItem;
        itemPath->setText(fileInfo.filePath());

        itemSuffix->setTextAlignment(Qt::AlignCenter);
        itemSize->setTextAlignment(Qt::AlignCenter);
        itemAllLine->setTextAlignment(Qt::AlignCenter);
        itemCommentLine->setTextAlignment(Qt::AlignCenter);
        itemBlank->setTextAlignment(Qt::AlignCenter);
        itemLine->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(i, 0, itemName);
        ui->tableWidget->setItem(i, 1, itemSuffix);
        ui->tableWidget->setItem(i, 2, itemSize);
        ui->tableWidget->setItem(i, 3, itemAllLine);
        ui->tableWidget->setItem(i, 4, itemLine);
        ui->tableWidget->setItem(i, 5, itemCommentLine);
        ui->tableWidget->setItem(i, 6, itemBlank);
        ui->tableWidget->setItem(i, 7, itemPath);

        totalBytes += fileInfo.size();
        totalLines += lineAll;
        totalCodes += lineCode;
        totalNotes += lineNotes;
        totalBlanks += lineBlank;

        if(i % 100 == 0)
        {
            qApp->processEvents();
        }
    }
    //显示统计结果
    m_listFile.clear();
    ui->lineEdit_fileCount->setText(QString::number(nCount));//文件数
    ui->lineEdit_byteCount->setText(QString::number(totalBytes));//文件总大小
    ui->lineEdit_totalLines->setText(QString::number(totalLines));//文件总行数
    ui->lineEdit_codeLines->setText(QString::number(totalCodes));//代码行数
    ui->lineEdit_commentLines->setText(QString::number(totalNotes));//注释行数
    ui->lineEdit_emptyLines->setText(QString::number(totalBlanks));//空白行数


    double percent = 0.0;
    //代码行所占百分比
    percent = ((double)totalCodes / totalLines) * 100;
    ui->label_percentCode->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));

    //注释行所占百分比
    percent = ((double)totalNotes / totalLines) * 100;
    ui->label_percentNotes->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
    //空行所占百分比
    percent = ((double)totalBlanks / totalLines) * 100;
    ui->label_percentBlank->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
    qDebug() << "files========================" << files;
}

void MainWindow::countCode(const QString &fileName, int &lineCode, int &lineBlank, int &lineNotes)
{
    lineCode = lineBlank = lineNotes = 0;
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream out(&file);
        QString line;
        bool isNote = false; //注释行标记
        while(!out.atEnd())
        {
            line = out.readLine();
            //qDebug() << "line=====" << line;
            //移除前面的空行
            if(line.startsWith(" "))
            {
                line.remove(" ");
            }
            //判断当前行是否为注释
            if(line.startsWith("/*"))
            {
                isNote = true;
            }
            //注释部分
            if(isNote)
            {
                lineNotes++;
            }
            else
            {
                if(line.startsWith("//"))
                {
                    lineNotes++;//注释行
                }
                else if(line.isEmpty())
                {
                    lineBlank++;//空白行
                }
                else
                {
                    lineCode++;//代码行
                }
            }
            //注释结束
            if(line.endsWith("*/"))
            {
                isNote = false;
            }
        }
    }
    qDebug() << "lineCode========================" << lineCode;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    //return;
    QPainter painter(this);
    painter.drawPixmap(QRect(0, 0, this->width(), this->height()), m_pixmap);
    //painter.drawText()
}


//https://www.cnblogs.com/feiyangqingyun/p/11669523.html
