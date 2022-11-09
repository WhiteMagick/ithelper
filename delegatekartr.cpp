#include "delegatekartr.h"
#include <QComboBox>
#include <QDebug>
#include <QSqlQuery>
#include <QApplication>

delegatekartr::delegatekartr(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *delegatekartr::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()==1){

    auto *editor = new QComboBox(parent);
        QSqlQuery query;
        query.prepare( "select id, name from SalaryKartridj;" );
        query.exec();

        while (query.next()) {

            editor->addItem(query.value(1).toString(), query.value(0).toInt());
        }
        return editor;
    }
}

void delegatekartr::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column()==1){
    auto *comboBox = dynamic_cast<QComboBox*>(editor);
    QString currentText = index.data(Qt::UserRole).toString();
    int cbIndex = comboBox->findText(currentText);
    if (cbIndex >= 0)
        comboBox->setCurrentIndex(cbIndex);
    }

}

void delegatekartr::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

    if (auto *comboBox = dynamic_cast<QComboBox*>(editor)) {
            QVariant modelData = comboBox->itemData(comboBox->currentIndex(), Qt::UserRole);    // id из Qt::UserRole
            model->setData(index, modelData.toString(), Qt::EditRole);
        }
        else
            QStyledItemDelegate::setModelData(editor, model, index);

}

void delegatekartr::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{


     editor->setGeometry(option.rect);
}

void delegatekartr::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column() == 1)
            {
                QStyleOptionComboBox comboBoxStyleOption;
                comboBoxStyleOption.state = option.state;
                comboBoxStyleOption.rect = option.rect;

                QSqlQuery qry;
                qry.prepare("select name from SalaryKartridj where id  = :id;");
                qry.bindValue(":id",index.data(Qt::EditRole).toString());
                qry.exec();
                qry.next();

                comboBoxStyleOption.currentText = qry.value(0).toString();

                QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &comboBoxStyleOption, painter, 0);
                QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel, &comboBoxStyleOption, painter, 0);

                return;
            }

    QStyledItemDelegate::paint(painter,option, index);
}

void delegatekartr::idcombo(int s)
{

}
