#include "simpledelegate.h"
#include <QColor>

SimpleDelegate::SimpleDelegate(QObject *pobj):QStyledItemDelegate(pobj){

}

void SimpleDelegate::paint(QPainter *pPainter, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    QModelIndex controlIndex = index.sibling( index.row(), 15);
    switch (controlIndex.data(Qt::DisplayRole).toInt()) {
    case 0:{

        break;
    }
    case 1: {
        QRect rect = option.rect;
        QLinearGradient gradient(0,0, rect.width(), rect.height());
        gradient.setColorAt(0,QColor(130,130,130));
        pPainter->setBrush(gradient);
        pPainter->drawRect(rect);
     QStyledItemDelegate::paint(pPainter,option,index);
        break;
    }
    case 2: {
        QRect rect = option.rect;
        QLinearGradient gradient(0,0, rect.width(), rect.height());
        gradient.setColorAt(0,QColor(207,207,207));
        pPainter->setBrush(gradient);
        pPainter->drawRect(rect);
     QStyledItemDelegate::paint(pPainter,option,index);
        break;
    }
    }

 QModelIndex controlST = index.sibling( index.row(), 20);
 switch (controlST.data(Qt::DisplayRole).toInt()) {
 case 0:{

     break;
 }
 case 1: {
     QRect rect = option.rect;
     QLinearGradient gradient(0,0, rect.width(), rect.height());
     gradient.setColorAt(0,QColor(255, 69, 0));
     pPainter->setBrush(gradient);
     pPainter->drawRect(rect);
  QStyledItemDelegate::paint(pPainter,option,index);
     break;
 }
 case 2: {
     QRect rect = option.rect;
     QLinearGradient gradient(0,0, rect.width(), rect.height());
     gradient.setColorAt(0,QColor(255, 255, 0));
     pPainter->setBrush(gradient);
     pPainter->drawRect(rect);
  QStyledItemDelegate::paint(pPainter,option,index);
     break;
 }

 }
// if(option.state & QStyle::State_MouseOver){
//        QRect rect = option.rect;
//        QLinearGradient gradient(0,0, rect.width(), rect.height());
//        gradient.setColorAt(0,Qt::lightGray);
//        pPainter->setBrush(gradient);
//        pPainter->drawRect(rect);
//    }
 QModelIndex controlVakansi = index.sibling( index.row(), 2);
if((controlVakansi.data().toString().indexOf("Вакансия")==0 && controlST.data(Qt::DisplayRole).toInt()==0)||(controlVakansi.data().toString().indexOf("вакансия")==0 && controlST.data(Qt::DisplayRole).toInt()==0)){
    QRect rect = option.rect;
    QLinearGradient gradient(0,0, rect.width(), rect.height());
    gradient.setColorAt(0,QColor(152, 251, 152));
    pPainter->setBrush(gradient);
    pPainter->drawRect(rect);
 QStyledItemDelegate::paint(pPainter,option,index);
}

    QStyledItemDelegate::paint(pPainter,option,index);





}

QSize SimpleDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex index) const
{
    return QSize(option.rect.width(),55);
}



