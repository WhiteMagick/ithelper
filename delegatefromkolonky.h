#ifndef DELEGATEFROMKOLONKY_H
#define DELEGATEFROMKOLONKY_H

#include <QtWidgets>
#include <QStyleOptionViewItem>

class delegatefromkolonky:public QStyledItemDelegate
{
public:

    delegatefromkolonky(QObject *pobj);
    void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex index) const;
};

#endif // DELEGATEFROMKOLONKY_H
