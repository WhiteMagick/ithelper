#include "addotherh.h"
#include "ui_addotherh.h"
#include <QSqlError>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QScreen>
#include <QKeyEvent>

addOtherH::addOtherH(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addOtherH)
{
    MdiArea = md;
    id1=id;
    ui->setupUi(this);
    ui->comboBox->clear();
    ui->comboBox->addItem("Не списан",QVariant(0));
    ui->comboBox->addItem("Списан",QVariant(1));
    ui->comboBox->addItem("На списании",QVariant(2));
    if(id1==0){
        setWindowTitle("Добавление записи");
    }
    else {
        setWindowTitle("Редактирование записи");

        qry.prepare("select  id, kab, name, model, inv, koment, STATUS from OtherHardware where id=:id;");
        qry.bindValue(":id",id1);
        qry.exec();
        qry.next();
        ui->lineEdit_kab->setText(qry.value(1).toString());
        ui->lineEdit_maker->setText(qry.value(2).toString());
        ui->lineEdit_model->setText(qry.value(3).toString());
        ui->lineEdit_inv->setText(qry.value(4).toString());
        ui->lineEdit_kom->setText(qry.value(5).toString());
        switch (qry.value(6).toInt()) {
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

addOtherH::~addOtherH()
{
    delete ui;
}

void addOtherH::on_pushButton_save_clicked()
{
    if(id1==0){

        if(!(ui->lineEdit_inv->text().length()>0)||!(ui->lineEdit_kab->text().length()>0)||!(ui->lineEdit_maker->text().length()>0)||!(ui->lineEdit_model->text().length()>0)){

            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery prov;
            prov.prepare("select inv from OtherHardware where inv = :inv;");
            prov.bindValue(":inv",ui->lineEdit_inv->text());
            prov.exec();
            prov.next();
            if(ui->lineEdit_inv->text()==prov.value(0).toString() && prov.value(0).toString()!="б/н"){
             QMessageBox::critical(this,"Error","Оборудование с таким инвентарным номером уже существует");
            }else {


    QSqlQuery qry2;
    qry2.prepare("INSERT INTO OtherHardware (kab, name, model, inv, koment, STATUS) "
                "VALUES (:kab, :name, :model, :inv, :koment, :STATUS);");
    qry2.bindValue(":kab",ui->lineEdit_kab->text());
    qry2.bindValue(":name",ui->lineEdit_maker->text());
    qry2.bindValue(":model",ui->lineEdit_model->text());
    qry2.bindValue(":inv",ui->lineEdit_inv->text());
    qry2.bindValue(":koment",ui->lineEdit_kom->text());
    qry2.bindValue(":STATUS",ui->comboBox->currentIndex());

    if(qry2.exec()){
        QSqlQuery del_log;
        del_log.prepare("exec delLog :tbl, :link;");
        del_log.bindValue(":tbl","OtherHardware");
        del_log.bindValue(":link",id1);
        del_log.exec();
        MdiArea->closeActiveSubWindow();
        emit Reselect();                                        //Вызывает слот для обновления таблици на экране

    }else {
        QMessageBox::critical(this,"Error",qry2.lastError().text()+"Все поля являются обязательными для заполнения");
            }
           }
        }

    }else{
        if(!(ui->lineEdit_inv->text().length()>0)||!(ui->lineEdit_kab->text().length()>0)||!(ui->lineEdit_maker->text().length()>0)||!(ui->lineEdit_model->text().length()>0)){
            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery prov;
            prov.prepare("select inv from OtherHardware where inv = :inv;");
            prov.bindValue(":inv",ui->lineEdit_inv->text());
            prov.exec();
            prov.next();
            if(ui->lineEdit_inv->text()==prov.value(0).toString()&& ui->lineEdit_inv->text()!=qry.value(4).toString() && prov.value(0).toString()!="б/н"){
             QMessageBox::critical(this,"Error","Оборудование с таким инвентарным номером уже существует");
            }else{

       QSqlQuery qryUp;
       qryUp.prepare("UPDATE OtherHardware SET kab=:kab, name=:name, model=:model, inv=:inv, koment=:koment, STATUS =:STATUS WHERE id=:id;");
       qryUp.bindValue(":id",id1);
       qryUp.bindValue(":kab",ui->lineEdit_kab->text());
       qryUp.bindValue(":name",ui->lineEdit_maker->text());
       qryUp.bindValue(":model",ui->lineEdit_model->text());
       qryUp.bindValue(":inv",ui->lineEdit_inv->text());
       qryUp.bindValue(":koment",ui->lineEdit_kom->text());
       qryUp.bindValue(":STATUS",ui->comboBox->currentIndex());

       if(qryUp.exec()){
           QSqlQuery del_log;
           del_log.prepare("exec delLog :tbl, :link;");
           del_log.bindValue(":tbl","OtherHardware");
           del_log.bindValue(":link",id1);
           del_log.exec();
           MdiArea->closeActiveSubWindow();
           emit Reselect();                                        //Вызывает слот  для обновления таблици на экране

       }else {
           QMessageBox::critical(this,"Error",qryUp.lastError().text());
       }
}
    }
    }
}

void addOtherH::on_pushButton_out_clicked()
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","OtherHardware");
    del_log.bindValue(":link",id1);
    del_log.exec();
     MdiArea->closeActiveSubWindow();
}

void addOtherH::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","OtherHardware");
    del_log.bindValue(":link",id1);
    del_log.exec();
    event->accept();
    MdiArea->activatePreviousSubWindow();
}

void addOtherH::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}
