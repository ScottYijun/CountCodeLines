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
        bool data = index.model()->data(index, Qt::DisplayRole).toBool();//取当前列对应的值
        //文件名  第0列
        QModelIndex index1 = index.model()->index(index.row(), 1, index.parent());
        QString fileName = index.model()->data(index1, Qt::DisplayRole).toString();
        //获到第1列，类型名
        QModelIndex index2 = index.model()->index(index.row(), 1, index.parent());
        QString suffix = index.model()->data(index2, Qt::DisplayRole).toString();

        //获取文件类型图标
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
        qDebug() << "option.rect==" << option.rect << "==row=====" << index.row() << "fileName==" << fileName
                 << " type==" << suffix << "pix.rect==" << pix.rect();
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
        qDebug() << "option.rect================" << option.rect << "==row=====" << index.row() << sizeof(checkBox);
        //画CheckBox
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxStyle, painter, &checkBox);
        //画图标
        QRect iconRect(checkBox.width() + 5, option.rect.y() + 6, 20, 20);
        painter->drawPixmap(iconRect, pix);
        //画文字
        QFont font("微软雅黑", 14, QFont::Bold, true);
        QFontMetrics fm = painter->fontMetrics();
        QString strElidedText = fm.elidedText(fileName, Qt::ElideRight, 100, Qt::TextShowMnemonic);
        //设置字体样式
        //font.setUnderline(true);//设置下划线
        //font.setOverline(true);//设置上划线
        //font.setCapitalization(QFont::SmallCaps);//设置字母大小写
        //font.setLetterSpacing(QFont::AbsoluteSpacing, 10);//设置字符间的间距
        painter->setFont(font);//使用字体
        painter->setPen(Qt::blue);//设置画笔记颜色
        painter->drawText(40, option.rect.y() + 20, strElidedText);
        qDebug() << "option.rect.y()=====================" << option.rect.y();
        //painter->translate(50, 50);//从位置（40， option.rect.y() + 20）移到位置（50，50），旋转的文本位置在这时设置
        //painter->rotate(90);//旋转90度
        //painter->drawText(0, 0, fileName);//绘制文本，旋转后前面两个坐标必须为0，否则显示不出来
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
































