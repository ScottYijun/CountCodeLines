#include "CItemDelelegate.h"
#include <QTextOption>
#include <QPainter>
#include <QCheckBox>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>
#include <QDebug>
#include <QApplication>
#include <QFileIconProvider>
#include <QIcon>
#include <QPixmap>
#include <QFileInfo>
#include <QFont>

#define CHECK_BOX_COLUMN 0

CItemDelegate::CItemDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}

void CItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if(option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    if(index.column() == CHECK_BOX_COLUMN)
    {
        bool data = index.model()->data(index, Qt::DisplayRole).toBool();//ȡ��ǰ�ж�Ӧ��ֵ
        //�ļ���  ��0��
        QModelIndex index1 = index.model()->index(index.row(), 1, index.parent());
        QString fileName = index.model()->data(index1, Qt::DisplayRole).toString();
        //�񵽵�1�У�������
        QModelIndex index2 = index.model()->index(index.row(), 1, index.parent());
        QString suffix = index.model()->data(index2, Qt::DisplayRole).toString();

        //��ȡ�ļ�����ͼ��
        QFileInfo fileInfo(fileName);
        QFileIconProvider iconProvider;
        //QIcon icon = iconProvider.icon(QFileIconProvider::File);
        QIcon icon = iconProvider.icon(fileInfo);
        QPixmap pix = icon.pixmap(QSize(20, 20));

        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;
        checkBoxStyle.iconSize = QSize(14, 14);
        checkBoxStyle.rect = option.rect;

        QCheckBox checkBox;
        checkBox.setMaximumSize(14, 14);
        checkBox.setMinimumSize(14, 14);
        checkBox.setStyleSheet("QCheckBox::indicator:unchecked {image: url(:/res/partiallyChecked.png);} \
                        QCheckBox::indicator:unchecked:hover {image: url(:/res/partiallyCheckedHover.png);} \
                        QCheckBox::indicator:unchecked:pressed {image: url(:/res/partiallyCheckedPressing.png);} \
                        QCheckBox::indicator:checked {image: url(:/res/checked.png);} \
                        QCheckBox::indicator:checked:hover {image: url(:/res/checkedHover.png);} \
                        QCheckBox::indicator:checked:pressed {image: url(:/res/checkedPressing.png);} \
                        QCheckBox::indicator:indeterminate {image: url(:/res/partiallyChecked.png);} \
                        QCheckBox::indicator:indeterminate:hover {image: url(:/res/partiallyCheckedHover.png);} \
                        QCheckBox::indicator:indeterminate:pressed {image: url(:/res/partiallyCheckedPressing.png);}");

        //��CheckBox
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxStyle, painter, &checkBox);
        //��ͼ��
        QRect iconRect(checkBox.width() + 5, option.rect.y() + 6, 20, 20);
        painter->drawPixmap(iconRect, pix);
        //������
        QFont font("΢���ź�", 14, QFont::Bold, true);
        QFontMetrics fm = painter->fontMetrics();
        QString strElidedText = fm.elidedText(fileName, Qt::ElideRight, 100, Qt::TextShowMnemonic);
        //����������ʽ
        //font.setUnderline(true);//�����»���
        //font.setOverline(true);//�����ϻ���
        //font.setCapitalization(QFont::SmallCaps);//������ĸ��Сд
        //font.setLetterSpacing(QFont::AbsoluteSpacing, 10);//�����ַ���ļ��
        painter->setFont(font);//ʹ������
        painter->setPen(Qt::blue);//���û��ʼ���ɫ
        //painter->drawText(40, option.rect.y() + 20, strElidedText);
        painter->drawText(40, option.rect.y() + 20, fileName);
        //painter->translate(50, 50);//��λ�ã�40�� option.rect.y() + 20���Ƶ�λ�ã�50��50������ת���ı�λ������ʱ����
        //painter->rotate(90);//��ת90��
        //painter->drawText(0, 0, fileName);//�����ı�����ת��ǰ�������������Ϊ0��������ʾ������
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool CItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{

    qDebug() << "CItemDelegate::editorEvent=====data================" << index.column();
    QRect decorationRect = option.rect;
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if(event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()))
    {
        if(index.column() == CHECK_BOX_COLUMN)
        {
            bool data = model->data(index, Qt::UserRole).toBool();
            qDebug() << "data================" << data;
            model->setData(index, !data, Qt::DisplayRole);//Qt::UserRole
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);

}
































