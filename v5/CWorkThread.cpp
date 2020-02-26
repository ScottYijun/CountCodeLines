#include "CWorkThread.h"
#include <QFileInfo>
#include <QDebug>
#include <QDir>

CWorkThread::CWorkThread(CTableModel *pTableModel)
    :m_pTableModel(pTableModel)
{

}

void CWorkThread::run()
{
    if(EN_COUNT_DIR_CODE == m_EnType)
    {
        filterFile(m_strPath);
        countFile(m_listFile);
    }
    else if(EN_COUNT_FILE_CODE == m_EnType)
    {
        countFile(m_listFile);
    }
}

bool CWorkThread::checkFile(const QString &fileName)
{
    if(fileName.startsWith("moc_") || fileName.startsWith("ui_") || fileName.startsWith("qrc_"))
    {
        return false;
    }

    QFileInfo file(fileName);//获取文件信息
    QString suffix = "*." + file.suffix();//文件后缀
    //QString filter = ui->lineEdit_filter->text().trimmed();//去掉前面空格
    //QStringList filters = filter.split(" ");
    QString filter = m_strFilterType.trimmed();
    QStringList filters = filter.split(" ");
    return filters.contains(suffix);
}


void CWorkThread::filterFile(const QString &filePath)
{
    qDebug() << "CWorkThread::filterFile======filePath=======" << filePath;
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
            filterFile(fileInfo.absoluteFilePath());
        }
    }

}

void CWorkThread::countFile(const QStringList &files)
{
    int lineCode;   //代码行
    int lineBlank;  //空行数
    int lineNotes;  //注释行数
    int nCount = files.count();
    qDebug() << "nCount=====================" << nCount;


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

        //m_listModel.append(tempFileInfo);
        m_pTableModel->addData(tempFileInfo);

        totalBytes += fileInfo.size();
        totalLines += lineAll;
        totalCodes += lineCode;
        totalNotes += lineNotes;
        totalBlanks += lineBlank;
        //每统计100个文件发一次信号
        if(i % 100 == 0)
        {
            emit signalUpdateData(i, totalBytes, totalLines, totalCodes, totalNotes, totalBlanks);
        }
    }
    //统计完成
    emit signalUpdateData(nCount, totalBytes, totalLines, totalCodes, totalNotes, totalBlanks);
//    //显示统计结果
//    m_pTableModel->updateData(m_listModel);
//    m_listFile.clear();
//    ui->lineEdit_fileCount->setText(QString::number(nCount));//文件数
//    ui->lineEdit_byteCount->setText(QString::number(totalBytes));//文件总大小
//    ui->lineEdit_totalLines->setText(QString::number(totalLines));//文件总行数
//    ui->lineEdit_codeLines->setText(QString::number(totalCodes));//代码行数
//    ui->lineEdit_noteLines->setText(QString::number(totalNotes));//注释行数
//    ui->lineEdit_blankLines->setText(QString::number(totalBlanks));//空白行数


//    double percent = 0.0;
//    //代码行所占百分比
//    percent = ((double)totalCodes / totalLines) * 100;
//    ui->label_percentCode->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));

//    //注释行所占百分比
//    percent = ((double)totalNotes / totalLines) * 100;
//    ui->label_percentNotes->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
//    //空行所占百分比
//    percent = ((double)totalBlanks / totalLines) * 100;
//    ui->label_percentBlank->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
//    qDebug() << "files========================" << files;
}

void CWorkThread::countCode(const QString &fileName, int &lineCode, int &lineBlank, int &lineNotes)
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

//设置统计目录代码的参数
void CWorkThread::setCountDirParam(const QString path, COUNTTYPE type, const QString filterType)
{
    m_strPath = path;
    m_EnType = type;
    m_strFilterType = filterType;
}

//设置统计文件代码的参数
void CWorkThread::setCountFileParam(const QStringList listFile, COUNTTYPE type, const QString filterType)
{
    m_listFile = listFile;
    m_EnType = type;
    m_strFilterType = filterType;
}
