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

    QFileInfo file(fileName);//��ȡ�ļ���Ϣ
    QString suffix = "*." + file.suffix();//�ļ���׺
    //QString filter = ui->lineEdit_filter->text().trimmed();//ȥ��ǰ��ո�
    //QStringList filters = filter.split(" ");
    QString filter = m_strFilterType.trimmed();
    QStringList filters = filter.split(" ");
    return filters.contains(suffix);
}


void CWorkThread::filterFile(const QString &filePath)
{
    qDebug() << "CWorkThread::filterFile======filePath=======" << filePath;
    QDir dir(filePath);
    foreach (QFileInfo fileInfo, dir.entryInfoList()) //entryInfoList()����QFileInfoList���͵Ķ���
    {
        if(fileInfo.isFile())
        {
            QString strFileName = fileInfo.fileName();
            //qDebug() << "strFileName========================" << strFileName;
            if(checkFile(strFileName)) //�������������ļ�
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
            //�ݹ��ҳ��ļ�
            filterFile(fileInfo.absoluteFilePath());
        }
    }

}

void CWorkThread::countFile(const QStringList &files)
{
    int lineCode;   //������
    int lineBlank;  //������
    int lineNotes;  //ע������
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
        //�ļ���
        tempFileInfo.fileName = fileInfo.fileName();

        //�ļ�����
        tempFileInfo.fileType = fileInfo.suffix();

        //�ļ���С
        tempFileInfo.fileSize = fileInfo.size();

        //����������
        tempFileInfo.totalLines = lineAll;

        //��������
        tempFileInfo.codeLines = lineCode;

        //ע������
        tempFileInfo.noteLines = lineNotes;

        //����
        tempFileInfo.blankLines = lineBlank;

        //�ļ�·��
        tempFileInfo.filePath = fileInfo.filePath();

        //m_listModel.append(tempFileInfo);
        m_pTableModel->addData(tempFileInfo);

        totalBytes += fileInfo.size();
        totalLines += lineAll;
        totalCodes += lineCode;
        totalNotes += lineNotes;
        totalBlanks += lineBlank;
        //ÿͳ��100���ļ���һ���ź�
        if(i % 100 == 0)
        {
            emit signalUpdateData(i, totalBytes, totalLines, totalCodes, totalNotes, totalBlanks);
        }
    }
    //ͳ�����
    emit signalUpdateData(nCount, totalBytes, totalLines, totalCodes, totalNotes, totalBlanks);
//    //��ʾͳ�ƽ��
//    m_pTableModel->updateData(m_listModel);
//    m_listFile.clear();
//    ui->lineEdit_fileCount->setText(QString::number(nCount));//�ļ���
//    ui->lineEdit_byteCount->setText(QString::number(totalBytes));//�ļ��ܴ�С
//    ui->lineEdit_totalLines->setText(QString::number(totalLines));//�ļ�������
//    ui->lineEdit_codeLines->setText(QString::number(totalCodes));//��������
//    ui->lineEdit_noteLines->setText(QString::number(totalNotes));//ע������
//    ui->lineEdit_blankLines->setText(QString::number(totalBlanks));//�հ�����


//    double percent = 0.0;
//    //��������ռ�ٷֱ�
//    percent = ((double)totalCodes / totalLines) * 100;
//    ui->label_percentCode->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));

//    //ע������ռ�ٷֱ�
//    percent = ((double)totalNotes / totalLines) * 100;
//    ui->label_percentNotes->setText(QString("%1%").arg(percent, 5, 'f', 2, QChar(' ')));
//    //������ռ�ٷֱ�
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
        bool isNote = false; //ע���б��
        while(!out.atEnd())
        {
            line = out.readLine();
            //qDebug() << "line=====" << line;
            //�Ƴ�ǰ��Ŀ���
            if(line.startsWith(" "))
            {
                line.remove(" ");
            }
            //�жϵ�ǰ���Ƿ�Ϊע��
            if(line.startsWith("/*"))
            {
                isNote = true;
            }
            //ע�Ͳ���
            if(isNote)
            {
                lineNotes++;
            }
            else
            {
                if(line.startsWith("//"))
                {
                    lineNotes++;//ע����
                }
                else if(line.isEmpty())
                {
                    lineBlank++;//�հ���
                }
                else
                {
                    lineCode++;//������
                }
            }
            //ע�ͽ���
            if(line.endsWith("*/"))
            {
                isNote = false;
            }
        }
    }
    qDebug() << "lineCode========================" << lineCode;
}

//����ͳ��Ŀ¼����Ĳ���
void CWorkThread::setCountDirParam(const QString path, COUNTTYPE type, const QString filterType)
{
    m_strPath = path;
    m_EnType = type;
    m_strFilterType = filterType;
}

//����ͳ���ļ�����Ĳ���
void CWorkThread::setCountFileParam(const QStringList listFile, COUNTTYPE type, const QString filterType)
{
    m_listFile = listFile;
    m_EnType = type;
    m_strFilterType = filterType;
}
