#include "CTableModel.h"
#include <QColor>

CTableModel::CTableModel()
{
    m_listFileInfo.clear();
}

//��������
void CTableModel::updateData(QList<FileInfor> &listFile)
{
    m_listFileInfo = listFile;
    beginResetModel();
    endResetModel();
}

//����
int CTableModel::rowCount(const QModelIndex &) const
{
    return m_listFileInfo.count();
}
//����
int CTableModel::columnCount(const QModelIndex &) const
{
    return 9;
}
//�����������ݣ����ݽ�ɫ����ɫ���ı������뷽ʽ��ѡ��״̬�ȣ��ж���Ҫ���õ����ݡ�
bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
            return false;

    int nColumn = index.column();
    int nRow = index.row();
    FileInfor fileInfo = m_listFileInfo.at(nRow);
//    switch (role)
//    {
//        case Qt::DisplayRole:
//        {
//            if (nColumn == File_PATH_COLUMN)
//            {
//                fileInfo.filePath = value.toString();

//                m_listFileInfo.replace(index.row(), record);
//                emit dataChanged(index, index);
//                return true;
//            }
//        }
//        case Qt::CheckStateRole:
//        {
//            if (nColumn == CHECK_BOX_COLUMN)
//            {
//                fileInfo.bChecked = (value.toInt() == Qt::Checked);

//                m_listFileInfo.replace(index.row(), record);
//                emit dataChanged(index, index);
//                return true;
//            }
//        }
//        default:
//            return false;
//    }
    return true;
}
//������ʾ���ݣ����ݽ�ɫ����ɫ���ı������뷽ʽ��ѡ��״̬�ȣ��ж���Ҫ��ʾ�����ݡ�
QVariant CTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int nRow = index.row();
    int nColumn = index.column();
    FileInfor fileInfo = m_listFileInfo.at(nRow);
    switch (role)
    {
        case Qt::TextColorRole:
            return QColor(Qt::red);
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case Qt::DisplayRole:
        {
            if(nColumn == FILE_CHECK_COLUMN)
                return fileInfo.bChecked;
            if(nColumn == FILE_NAME_COLUMN)
                return fileInfo.fileName;
            if(nColumn == FIlE_TYPE_COLOUMN)
                return fileInfo.fileType;
            if(nColumn == FILE_SIZE_COLUMN)
                return fileInfo.fileSize;
            if(nColumn == TOTAL_LINES_COLUMN)
                return fileInfo.totalLines;
            if(nColumn == CODE_LINES_COLUMN)
                return fileInfo.codeLines;
            if(nColumn == NOTE_LINES_COLOUMN)
                return fileInfo.noteLines;
            if(nColumn == BLANK_LINES_COLUMN)
                return fileInfo.blankLines;
            if(nColumn == FILE_PATH_COLUMN)
                return fileInfo.filePath;
            return QVariant();
        }
//        case Qt::CheckStateRole:
//        {
//            if(nColumn == 0)
//                return fileInfo.bChecked ? Qt::Checked : Qt::Unchecked;
//        }
        default:
            return QVariant();
    }
}
//������ʾˮƽ/��ֱ��ͷ�����ݡ�
QVariant CTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case Qt::DisplayRole:
        {
            if(orientation == Qt::Horizontal)
            {
                if(section == FILE_CHECK_COLUMN)
                    return QString(tr("file name"));
                if(section == FILE_NAME_COLUMN)
                    return QString(tr("file name"));
                if(section == FIlE_TYPE_COLOUMN)
                    return QString(tr("type"));
                if(section == FILE_SIZE_COLUMN)
                    return QString(tr("size"));
                if(section == TOTAL_LINES_COLUMN)
                    return QString(tr("total lines"));
                if(section == CODE_LINES_COLUMN)
                    return QString(tr("code lines"));
                if(section == NOTE_LINES_COLOUMN)
                    return QString(tr("note lines"));
                if(section == BLANK_LINES_COLUMN)
                    return QString(tr("blank lines"));
                if(section == FILE_PATH_COLUMN)
                    return QString(tr("path"));
            }
        }
        default:
            return QVariant();
    }

}
//�������õ�Ԫ��ı�־�����á���ѡ�С��ɸ�ѡ�ȣ���
Qt::ItemFlags CTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return QAbstractTableModel::flags(index);

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(index.column() == 0)
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}















