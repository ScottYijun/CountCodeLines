#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <numeric>
#include <QPainter>
#include "CItemDelelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pTableModel = new CTableModel();
    ui->tableView->setModel(m_pTableModel);
    CItemDelegate *itemDelegate = new CItemDelegate(ui->tableView);
    //ui->tableView->setItemDelegate(itemDelegate);
    //设置第0列委托
    ui->tableView->setItemDelegateForColumn(0, itemDelegate);
    m_pixmap.load(":/background.jpg");
    m_pixmap = m_pixmap.scaled(800, 600);
    m_listModel.clear();
    //this->setToolTip(tr("code line statistics tool"));
    this->setWindowTitle(tr("code line statistics tool"));

    initView();
    initTableView();

    connect(ui->pushButton_openDir, SIGNAL(clicked()), this, SLOT(slotOpenDir()));
    connect(ui->pushButton_openFile, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(ui->pushButton_clearResult, SIGNAL(clicked()), this, SLOT(slotClearResult()));
    connect(ui->label_fileName, SIGNAL(linkActivated(QString)), this, SLOT(slotLabelLink(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotLabelLink(const QString &link)
{
    qDebug() << "MainWindow::slotLabel========line=====" << link;
}

void MainWindow::initView()
{
    //ui->label_fileName->setText("<a style='color:blue' href=https://www.csdn.net>csdn</a>");

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
    //ui->tableView->setStyleSheet( "QTableView{background: transparent;}" );
    ui->lineEdit_filter->setToolTip("输入文件类型时用空格隔开，例如：*.h *.cpp *.c");
}

void MainWindow::initTableView()
{
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->verticalHeader()->setDefaultSectionSize(35);//设置行高
    ui->tableView->verticalHeader()->setHighlightSections(false);

  
    //设置列宽
    ui->tableView->setColumnWidth(0, 180);
    //隐藏第一列
    ui->tableView->setColumnHidden(1, true);


    //设置前景色
    ui->lineEdit_fileCount->setStyleSheet("color:#17A086;");
    ui->lineEdit_byteCount->setStyleSheet("color:#CA5AA6;");
    ui->lineEdit_totalLines->setStyleSheet("color:#CD1B19;");
    ui->lineEdit_codeLines->setStyleSheet("color:#22A3A9;");
    ui->lineEdit_blankLines->setStyleSheet("color:#D64D54;");
    ui->lineEdit_noteLines->setStyleSheet("color:#A279C5;");

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
    ui->lineEdit_blankLines->setFont(font);
    ui->lineEdit_noteLines->setFont(font);

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
    ui->lineEdit_noteLines->setText("0");//注释行数
    ui->lineEdit_blankLines->setText("0");//空白行数

    ui->label_percentCode->setText("0%");
    ui->label_percentNotes->setText("0%");
    ui->label_percentBlank->setText("0%");
}


//找出目录下所有符合条件的文件
void MainWindow::countCode(const QString &filePath)
{
    QDir dir(filePath);
    foreach (QFileInfo fileInfo, dir.entryInfoList()) //entryInfoList()返回QFileInfoList类型的对象
    {
        if(fileInfo.isFile())
        {
            QString strFileName = fileInfo.fileName();
           
            if(checkFile(strFileName)) //检测符合条件的文件
            {
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
    
    quint32 totalLines = 0;
    quint32 totalBytes = 0;
    quint32 totalCodes = 0;
    quint32 totalNotes = 0;
    quint32 totalBlanks = 0;

    for(int i = 0; i < nCount; ++i)
    {
        QFileInfo fileInfo(files.at(i));
        countCode(fileInfo.filePath(), lineCode, lineBlank, lineNotes);

        int lineAll = lineCode + lineBlank + lineNotes;
        FileInfor tempFileInfo;
        tempFileInfo.bChecked = false;
        //文件名
        tempFileInfo.fileName = fileInfo.fileName();

        //文件类型
        tempFileInfo.fileType = fileInfo.suffix();

        //文件大小
        tempFileInfo.fileSize = fileInfo.size();

        //代码总行数
        tempFileInfo.totalLines = lineAll;

        //代码行数
        tempFileInfo.codeLines = lineCode;

        //注释行数
        tempFileInfo.noteLines = lineNotes;

        //空行
        tempFileInfo.blankLines = lineBlank;

        //文件路径
        tempFileInfo.filePath = fileInfo.filePath();

        m_listModel.append(tempFileInfo);

        totalBytes += fileInfo.size();
        totalLines += lineAll;
        totalCodes += lineCode;
        totalNotes += lineNotes;
        totalBlanks += lineBlank;

    }
    //显示统计结果
    m_pTableModel->updateData(m_listModel);
    m_listFile.clear();
    ui->lineEdit_fileCount->setText(QString::number(nCount));//文件数
    ui->lineEdit_byteCount->setText(QString::number(totalBytes));//文件总大小
    ui->lineEdit_totalLines->setText(QString::number(totalLines));//文件总行数
    ui->lineEdit_codeLines->setText(QString::number(totalCodes));//代码行数
    ui->lineEdit_noteLines->setText(QString::number(totalNotes));//注释行数
    ui->lineEdit_blankLines->setText(QString::number(totalBlanks));//空白行数


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
   
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(QRect(0, 0, this->width(), this->height()), m_pixmap);
}


//https://www.cnblogs.com/feiyangqingyun/p/11669523.html
