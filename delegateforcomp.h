#ifndef DELEGATEFORCOMP_H
#define DELEGATEFORCOMP_H

#include <QtWidgets>
#include <QStyleOptionViewItem>

class DelegateForComp:public QStyledItemDelegate
{
public:
    DelegateForComp(QObject *pobj);
    void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex index) const;
};

#endif // DELEGATEFORCOMP_H
