#include "addprn.h"
#include "ui_addprn.h"
#include <QSqlError>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QScreen>
#include <QStringList>
#include <QKeyEvent>


AddPrn::AddPrn(int id,int ot, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPrn)
{
    id1=id;
    MdiAreaPrn = md;
    otdel=ot;
    ui->setupUi(this);
    ui->comboBox->clear();
    ui->comboBox->addItem("Не списан",QVariant(0));
    ui->comboBox->addItem("Списан",QVariant(1));
    ui->comboBox->addItem("На списании",QVariant(2));


    fillcombo();

    if(id1==0){
        setWindowTitle("Добавление записи");
    }
    else {
        setWindowTitle("Редактирование записи");

        qry.prepare("select p.id, p.otdelid, p.kab as 'Кабинет', p.model as 'Модель', p.name as 'Имя', p.IP, p.inv as 'Инвентарный номер',"
                    "p.ser as 'Серийный номер', p.roz as 'Розетка', p.comment as 'Коммернтарий',p.spisan, sl.model, m.maker,sl.id from Printer p "
                    "left join SalaryPrn sl on sl.id = p.model "
                    "left join SalaryMakerPRN m on sl.maker=m.id "
                    "where p.id=:id;");
        qry.bindValue(":id",id1);
        qry.exec();
        qry.next();
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->kabinet->setText(qry.value(2).toString());
        ui->comboBox_3->setCurrentIndex(ui->comboBox_3->findText(qry.value(12).toString()));
        ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findText(qry.value(11).toString()));
        ui->name->setText(qry.value(4).toString());
        ui->ip->setText(qry.value(5).toString());
        ui->inv->setText(qry.value(6).toString());
        ui->ser->setText(qry.value(7).toString());
        ui->roz->setText(qry.value(8).toString());
        ui->lineEdit->setText(qry.value(9).toString());

        switch (qry.value(10).toInt()) {
        case 0:{
            ui->comboBox->setCurrentIndex(0);
            break;
        }
        case 1:{
            ui->comboBox->setCurrentIndex(1);
            break;
        }
        case 2:{
            ui->comboBox->setCurrentIndex(2);
            break;
        }


    }
}
}
AddPrn::~AddPrn()
{
    delete ui;
}

void AddPrn::fillcombo()
{
    QSqlQuery slMk;
    slMk.prepare("select maker,id from SalaryMakerPRN;");
    slMk.exec();
    while(slMk.next()){
        ui->comboBox_3->addItem(slMk.value(0).toString(),QVariant(slMk.value(1).toInt()));
    }

    QSqlQuery sl;
    sl.prepare("select model,id,maker from SalaryPrn where maker = :id;");
    sl.bindValue(":id",ui->comboBox_3->itemData(ui->comboBox_3->currentIndex()).toInt());
    sl.exec();
    while(sl.next()){
        ui->comboBox_2->addItem(sl.value(0).toString(),QVariant(sl.value(1).toInt()));
    }
}

void AddPrn::fillcomboforEdit()
{
    QSqlQuery slMk;
    slMk.prepare("select maker,id from SalaryMakerPRN;");
    slMk.exec();
    while(slMk.next()){
        ui->comboBox_3->addItem(slMk.value(0).toString(),QVariant(slMk.value(1).toInt()));
    }

    QSqlQuery sl;
    sl.prepare("select model,id,maker from SalaryPrn where maker = :id;");
    sl.bindValue(":id",ui->comboBox_3->itemData(ui->comboBox_3->currentIndex()).toInt());
    sl.exec();
    while(sl.next()){
        ui->comboBox_2->addItem(sl.value(0).toString(),QVariant(sl.value(1).toInt()));
    }
}

void AddPrn::on_pushButton_2_clicked()
{
    if(id1==0){

        if(!(ui->kabinet->text().length()>0)||!(ui->name->text().length()>0)||!(ui->inv->text().length()>0)||!(ui->ser->text().length()>0)){

            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery prov;
            prov.prepare("select inv from Printer where inv = :inv;");
            prov.bindValue(":inv",ui->inv->text());
            prov.exec();
            prov.next();
            if(ui->inv->text()==prov.value(0).toString()){
             QMessageBox::critical(this,"Error","Принтер с таким инвентарным номером уже существует");
            }else {


    QSqlQuery qry2;
    qry2.prepare("INSERT INTO Printer (otdelid, kab, model, name, IP, inv, ser, roz, comment, spisan, STATUS) "
                "VALUES (:otdelid, :kab, :model, :name, :IP, :inv, :ser, :roz, :comment, :spisan, 0);");
    qry2.bindValue(":otdelid",otdel);
    qry2.bindValue(":kab",ui->kabinet->text());
    qry2.bindValue(":model",ui->comboBox_2->itemData(ui->comboBox_2->currentIndex()).toInt());
    qry2.bindValue(":name",ui->name->text());
    qry2.bindValue(":IP",ui->ip->text());
    qry2.bindValue(":inv",ui->inv->text());
    qry2.bindValue(":ser",ui->ser->text());
    qry2.bindValue(":roz",ui->roz->text());
    qry2.bindValue(":comment",ui->lineEdit->text());
    qry2.bindValue(":spisan",ui->comboBox->currentIndex());


    if(qry2.exec()){
        MdiAreaPrn->closeActiveSubWindow();
        emit Reselect();                                        //Вызывает слот для обновления таблици на экране

    }else {
        QMessageBox::critical(this,"Error",qry2.lastError().text()+"Все поля являются обязательными для заполнения");
            }
           }
        }

    }else{
        if(!(ui->kabinet->text().length()>0)||!(ui->name->text().length()>0)||!(ui->inv->text().length()>0)||!(ui->ser->text().length()>0)){
            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery prov;
            prov.prepare("select inv from Comp where inv = :inv;");
            prov.bindValue(":inv",ui->inv->text());
            prov.exec();
            prov.next();
            if(ui->inv->text()==prov.value(0).toString() && ui->inv->text()!=qry.value(4).toString()){
             QMessageBox::critical(this,"Error","Принтер с таким инвентарным номером уже существует");
            }else{

       QSqlQuery qryUp;
       qryUp.prepare("UPDATE Printer SET otdelid=:otdelid, kab=:kab, model=:model, name=:name, IP=:IP, inv=:inv, ser=:ser, roz=:roz, comment=:comment, spisan=:spisan WHERE id=:id;");
       qryUp.bindValue(":id",id1);
       qryUp.bindValue(":otdelid",otdel);
       qryUp.bindValue(":kab",ui->kabinet->text());
       qryUp.bindValue(":model",ui->comboBox_2->itemData(ui->comboBox_2->currentIndex()).toInt());
       qryUp.bindValue(":name",ui->name->text());
       qryUp.bindValue(":IP",ui->ip->text());
       qryUp.bindValue(":inv",ui->inv->text());
       qryUp.bindValue(":ser",ui->ser->text());
       qryUp.bindValue(":roz",ui->roz->text());
       qryUp.bindValue(":comment",ui->lineEdit->text());
       qryUp.bindValue(":spisan",ui->comboBox->currentIndex());

       if(qryUp.exec()){
           QSqlQuery del_log;
           del_log.prepare("exec delLog :tbl, :link;");
           del_log.bindValue(":tbl","Printer");
           del_log.bindValue(":link",id1);
           del_log.exec();
           MdiAreaPrn->closeActiveSubWindow();
           emit Reselect();                                        //Вызывает слот  для обновления таблици на экране

       }else {
           QMessageBox::critical(this,"Error",qryUp.lastError().text());
       }
}
    }
    }

}

void AddPrn::on_pushButton_clicked()
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","Printer");
    del_log.bindValue(":link",id1);
    del_log.exec();
    MdiAreaPrn->closeActiveSubWindow();
}

void AddPrn::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","Printer");
    del_log.bindValue(":link",id1);
    del_log.exec();
    event->accept();
    MdiAreaPrn->activatePreviousSubWindow();

}

void AddPrn::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

      }

    }



void AddPrn::on_comboBox_3_currentIndexChanged(int index)
{
    ui->comboBox_2->clear();
    QSqlQuery sl;
    sl.prepare("select model,id,maker from SalaryPrn where maker = :id;");
    sl.bindValue(":id",ui->comboBox_3->itemData(ui->comboBox_3->currentIndex()).toInt());
    sl.exec();
    while(sl.next()){
        ui->comboBox_2->addItem(sl.value(0).toString(),QVariant(sl.value(1).toInt()));
    }
}
