#include "delegateforprinter.h"

DelegateForPrinter::DelegateForPrinter(QObject *pobj):QStyledItemDelegate(pobj)
{

}

void DelegateForPrinter::paint(QPainter *pPainter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QModelIndex controlIndex = index.sibling( index.row(), 12);
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
    case 2: {
        QRect rect = option.rect;
        QLinearGradient gradient(0,0, rect.width(), rect.height());
        gradient.setColorAt(0,QColor(255, 69, 0));
        pPainter->setBrush(gradient);
        pPainter->drawRect(rect);
     QStyledItemDelegate::paint(pPainter,option,index);
        break;
    }

    }

QStyledItemDelegate::paint(pPainter,option,index);
}

QSize DelegateForPrinter::sizeHint(const QStyleOptionViewItem &option, const QModelIndex index) const
{
return QSize(option.rect.width(),55);
}
