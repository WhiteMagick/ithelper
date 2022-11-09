#include "viborotdela.h"
#include "ui_viborotdela.h"
#include <QDebug>
#include <QDesktopServices>
#include <QChar>
#include <windows.h>
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QSqlError>
#include <QKeyEvent>
ViborOtdela::ViborOtdela(QMdiArea *md, int id, int who, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViborOtdela)
{
    mdiAreaVibor = md;
    ot=id;
    where= who;
    ui->setupUi(this);
    setWindowTitle("Выбор отдела");
    ui->treeWidget->clear();
    //создание списка отделов и вывод в дерево

    QString sSql = QString("select name from Otdel order by CODE ASC;");
        QSqlQuery parentsQuery;
        if(!parentsQuery.exec(sSql))
            return;
        while(parentsQuery.next())
            {

                QString parentName = parentsQuery.value(0).toString();

                QTreeWidgetItem* parentItem = new QTreeWidgetItem();
                parentItem->setText(0,parentName);
                ui->treeWidget->setColumnCount(1);
                ui->treeWidget->headerItem()->setHidden(1);
                ui->treeWidget->addTopLevelItem(parentItem);
                parentItem->setIcon(0,QIcon(":/sub.png"));

        }
}

ViborOtdela::~ViborOtdela()
{
    delete ui;
}

void ViborOtdela::on_pushButton_clicked()
{
    if(ot==0){
        QMessageBox::about(this,"Error","Не выбрана запись!");
        mdiAreaVibor->closeActiveSubWindow();
    }else{
    QModelIndex currentDiscount  = ui->treeWidget->currentIndex();
    QString n = ui->treeWidget->model()->data(ui->treeWidget->model()->index(currentDiscount.row(),0)).toString();
    QSqlQuery idot;
    idot.prepare("select CODE from Otdel where name = :nam;");
    idot.bindValue(":nam",n);
    idot.exec();
   idot.next();
   switch (where) {
   case 0:{
       qry.prepare("update perepis set OTDELID =:ot where id = :id;");
       qry.bindValue(":ot",idot.value(0).toInt());
       qry.bindValue(":id",ot);
       qry.exec();
       emit Reselect();
       mdiAreaVibor->closeActiveSubWindow();
       break;
        }
   case 1:{
       qry.prepare("update Printer set otdelid =:ot where id = :id;");
       qry.bindValue(":ot",idot.value(0).toInt());
       qry.bindValue(":id",ot);
       qry.exec();
       emit Reselect();
       mdiAreaVibor->closeActiveSubWindow();
       break;
           }
   }


   }



}

void ViborOtdela::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(ot==0){
        QMessageBox::about(this,"Error","Не выбрана запись!");
        mdiAreaVibor->closeActiveSubWindow();
    }else{
    st=item->text(0);
    QSqlQuery idot;
    idot.prepare("select CODE from Otdel where name = :nam;");
    idot.bindValue(":nam",st);
    idot.exec();
    idot.next();
    switch (where) {
    case 0:{
        qry.prepare("update perepis set OTDELID =:ot where id = :id;");
        qry.bindValue(":ot",idot.value(0).toInt());
        qry.bindValue(":id",ot);
        qry.exec();
        emit Reselect();
        mdiAreaVibor->closeActiveSubWindow();
        break;
         }
    case 1:{
        qry.prepare("update Printer set otdelid =:ot where id = :id;");
        qry.bindValue(":ot",idot.value(0).toInt());
        qry.bindValue(":id",ot);
        qry.exec();
        emit Reselect();
        mdiAreaVibor->closeActiveSubWindow();
        break;
            }
    }

}


}

void ViborOtdela::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}
