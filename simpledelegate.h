#ifndef SIMPLEDELEGATE_H
#define SIMPLEDELEGATE_H

#include <QtWidgets>
#include <QStyleOptionViewItem>

class SimpleDelegate:public QStyledItemDelegate
{
public:
    SimpleDelegate(QObject* pobj =0);
    void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex index) const;


};

#endif // SIMPLEDELEGATE_H
