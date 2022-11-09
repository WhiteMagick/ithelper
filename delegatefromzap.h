#ifndef DELEGATEFROMZAP_H
#define DELEGATEFROMZAP_H


#include <QtWidgets>
#include <QStyleOptionViewItem>

class delegatefromZap : public QStyledItemDelegate
{
public:

    delegatefromZap(QObject *pobj);
    void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex index) const;
};

#endif // DELEGATEFROMZAP_H
