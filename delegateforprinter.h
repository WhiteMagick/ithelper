#ifndef DELEGATEFORPRINTER_H
#define DELEGATEFORPRINTER_H


#include <QtWidgets>
#include <QStyleOptionViewItem>

class DelegateForPrinter:public QStyledItemDelegate
{
public:
    DelegateForPrinter(QObject* pobj =0);
    void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex index) const;
};

#endif // DELEGATEFORPRINTER_H
