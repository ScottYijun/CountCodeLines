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
    m_pWorkThread = nullptr;
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
    initData();
    initConnect();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initData()
{
    if(nullptr == m_pWorkThread)
    {
        m_pWorkThread = new CWorkThread(m_pTableModel);
        connect(m_pWorkThread, SIGNAL(signalUpdateData(int, quint32, quint32, quint32, quint32, quint32)),
                this, SLOT(slotUpdateData(int, quint32, quint32, quint32, quint32, quint32)));
    }
}

void MainWindow::initConnect()
{
    connect(ui->pushButton_openDir, SIGNAL(clicked()), this, SLOT(slotOpenDir()));
    connect(ui->pushButton_openFile, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(ui->pushButton_clearResult, SIGNAL(clicked()), this, SLOT(slotClearResult()));
    connect(ui->label_fileName, SIGNAL(linkActivated(QString)), this, SLOT(slotLabelLink(QString)));
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
    //ui->lineEdit_filter->setToolTip("输入文件类型时用空格隔开，例如：*.h *.cpp *.c");
    ui->lineEdit_filter->setToolTip(tr("Separated by spaces when entering file types, eg: *.h *.cpp *.c"));
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
    ui->tableView->setColumnWidth(0, 180);//设置列宽
    ui->tableView->setColumnHidden(1, true);//隐藏第一列


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



void MainWindow::slotOpenDir()
{
    QString path =  QFileDialog::getExistingDirectory(this, tr("choose dir"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!path.isEmpty())
    {
        ui->lineEdit_dir->setText(path);
        m_listFile.clear();
        slotClearResult();
        //countCode(path);
        //qDebug() << "m_listFile======" << m_listFile;
        //countCode(m_listFile);
        m_pWorkThread->setCountDirParam(path, EN_COUNT_DIR_CODE, ui->lineEdit_filter->text().trimmed());
        m_pWorkThread->start();
    }
}

void MainWindow::slotOpenFile()
{
    QString filter = QString(tr("code file(%1)").arg(ui->lineEdit_filter->text().trimmed()));
    QStringList files = QFileDialog::getOpenFileNames(this, tr("choose file"), "./", filter);
    if(files.size() > 0)
    {
        ui->lineEdit_fileName->setText(files.join("|"));
        slotClearResult();
        //countCode(files);
        m_pWorkThread->setCountFileParam(files, EN_COUNT_FILE_CODE, ui->lineEdit_filter->text().trimmed());
        m_pWorkThread->start();
    }
}

void MainWindow::slotClearResult()
{
    m_pTableModel->clearData();
    m_listFile.clear();
    m_pTableModel->updateData();
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

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(QRect(0, 0, this->width(), this->height()), m_pixmap);
}

void MainWindow::slotUpdateData(int nFileCount, quint32 totalBytes, quint32 totalLines, quint32 totalCodes, quint32 totalNotes, quint32 totalBlanks)
{
    ui->lineEdit_fileCount->setText(QString::number(nFileCount));//文件数
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
    m_pTableModel->updateData();
}

//http://www.cnblogs.com/zhoug2020/p/3789076.html
//https://www.cnblogs.com/feiyangqingyun/p/11669523.html
