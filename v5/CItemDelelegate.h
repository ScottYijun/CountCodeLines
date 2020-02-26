#ifndef CITEMDELEGATE_H
#define CITEMDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>

class CItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    CItemDelegate(QObject *parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &ption, const QModelIndex &index) const;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

};

#endif // CITEMDELEGATE_H

//Reimplemented Public Functions

//virtual QWidget *
//createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
//virtual void
//paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
//virtual void
//setEditorData(QWidget *editor, const QModelIndex &index) const override
//virtual void
//setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
//virtual QSize
//sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
//virtual void
//updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override

//Protected Functions

//virtual void
//initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const

//Reimplemented Protected Functions

//virtual bool
//editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override
//virtual bool
//eventFilter(QObject *editor, QEvent *event) override

































