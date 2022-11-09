#include "delegatefromkolonky.h"

delegatefromkolonky::delegatefromkolonky(QObject *pobj)
{

}

void delegatefromkolonky::paint(QPainter *pPainter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QModelIndex controlIndex = index.sibling( index.row(), 9);
    switch (controlIndex.data(Qt::DisplayRole).toInt()) {
    case 0:{

        break;
    }
    case 1: {
        QRect rect = option.rect;
        QLinearGradient gradient(0,0, rect.width(), rect.height());
        gradient.setColorAt(0,QColor(255, 255, 0));
        pPainter->setBrush(gradient);
        pPainter->drawRect(rect);
     QStyledItemDelegate::paint(pPainter,option,index);
        break;
    }
    }
    QStyledItemDelegate::paint(pPainter,option,index);

}

QSize delegatefromkolonky::sizeHint(const QStyleOptionViewItem &option, const QModelIndex index) const
{
 return QSize(option.rect.width(),55);
}
