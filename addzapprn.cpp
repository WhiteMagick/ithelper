#include "addzapprn.h"
#include "ui_addzapprn.h"
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
addZapPrn::addZapPrn(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addZapPrn)
{
    ui->setupUi(this);
    MdiAreaAddZap = md;
    id1 = id;
    createChek();
    if(id1==0){
        setWindowTitle("Добавление записи");
        //createChek();
    }else{
        setWindowTitle("Редактирование записи");


    }

}

addZapPrn::~addZapPrn()
{
    delete ui;

}

void addZapPrn::createChek()
{

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setShowGrid(true);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeMode::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeMode::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<" "<<" "<<"Наименование");
//    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    listCheck.clear();
    qry.prepare("SELECT id, model FROM SalaryPrn;");
    qry.exec();
    ui->tableWidget->hideColumn(0);
    int i=0;
    while(qry.next()){
    QWidget *chechBoxWidget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();
    QHBoxLayout *layoutCheckBox = new QHBoxLayout(chechBoxWidget);
    layoutCheckBox->addWidget(checkBox);
    layoutCheckBox->setAlignment(Qt::AlignCenter);
    layoutCheckBox->setContentsMargins(0,0,0,0);
    checkBox->setObjectName(QString::number(i));
    checkBox->setChecked(false);
    listCheck.append(checkBox);
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setCellWidget(i,1, chechBoxWidget);
    ui->tableWidget->setItem(i,0,new QTableWidgetItem(qry.value(0).toString()));
    ui->tableWidget->setItem(i,2,new QTableWidgetItem(qry.value(1).toString()));
    if(id1!=0){
        QSqlQuery list;
        list.prepare("select listPrn, name, count, comment  from ZapchastiPrn where id = :id;");
        list.bindValue(":id",id1);
        list.exec();
        list.next();
        ui->lineEdit->setText(list.value(1).toString());
        ui->lineEdit_2->setText(list.value(2).toString());
        ui->textEdit->setPlainText(list.value(3).toString());
        QString l = list.value(0).toString();
        QStringList list2 = l.split(QLatin1Char(','));
        for(int i =0; i<list2.size();++i){

               if( qry.value(0).toString()==list2.at(i)){
                 checkBox->setChecked(true);
            }

    }
   }
    i++;
    }



}

void addZapPrn::on_pushButton_2_clicked()  //сохранить
{

    QStringList l;
    l.clear();
    QString s;
    for(int i =0; i<listCheck.length();i++){
        if(listCheck[i]->isChecked()==true){
          l.append(ui->tableWidget->item(listCheck[i]->objectName().toInt(),0)->text());
        }
    }

    QStringList::const_iterator constIterator;
        for (constIterator = l.constBegin(); constIterator != l.constEnd(); ++constIterator){
                  s=s+ (*constIterator).toLocal8Bit().constData()+",";
        }
if(id1==0){
  if(ui->lineEdit->text().length()>0 && ui->lineEdit_2->text().length()>0){
    qDebug()<<"ggg";
        QSqlQuery insert;
        insert.prepare("INSERT INTO  ZapchastiPrn (name, count, listPrn, comment, status_zap) VALUES (:name, :count, :listPrn, :comment, :status_zap );");
        insert.bindValue(":name",ui->lineEdit->text());
        insert.bindValue(":count",ui->lineEdit_2->text());
        insert.bindValue(":listPrn",s.left(s.lastIndexOf(QChar(','))));
        insert.bindValue(":comment",ui->textEdit->toPlainText());
        insert.bindValue(":status_zap","0");

        if(!insert.exec()){
            insert.lastError().text();
        }

        emit Reselect();
        MdiAreaAddZap->closeActiveSubWindow();
        MdiAreaAddZap->activatePreviousSubWindow();
  }else {
    QMessageBox::critical(this,"Error","Все поля являются обязательными для заполнения");
        }
    }else{

        if(ui->lineEdit->text().length()>0 && ui->lineEdit_2->text().length()>0){
           QSqlQuery up;
           up.prepare("UPDATE ZapchastiPrn SET name=:name, count=:count, listPrn=:listPrn, comment=:comment where id = :id;");
           up.bindValue(":id",id1);
           up.bindValue(":name",ui->lineEdit->text());
           up.bindValue(":count",ui->lineEdit_2->text());
           up.bindValue(":listPrn",s.left(s.lastIndexOf(QChar(','))));
           up.bindValue(":comment",ui->textEdit->toPlainText());
           if(!up.exec()){
                       up.lastError().text();
                   }
           QSqlQuery del_log;
           del_log.prepare("exec delLog :tbl, :link;");
           del_log.bindValue(":tbl","ZapchastiPrn");
           del_log.bindValue(":link",id1);
           del_log.exec();
                   emit Reselect();
                   MdiAreaAddZap->closeActiveSubWindow();
                   MdiAreaAddZap->activatePreviousSubWindow();

        }else {
            QMessageBox::critical(this,"Error","Все поля являются обязательными для заполнения");
                }
    }

}

void addZapPrn::createlist()
{


}

void addZapPrn::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","ZapchastiPrn");
    del_log.bindValue(":link",id1);
    del_log.exec();
    event->accept();
    MdiAreaAddZap->activatePreviousSubWindow();
}

void addZapPrn::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

      }


}

void addZapPrn::on_pushButton_clicked()
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","ZapchastiPrn");
    del_log.bindValue(":link",id1);
    del_log.exec();
    MdiAreaAddZap->closeActiveSubWindow();
    MdiAreaAddZap->activatePreviousSubWindow();
}
