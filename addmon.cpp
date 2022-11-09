#include "addmon.h"
#include "ui_addmon.h"
#include <QSqlError>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QScreen>
#include <QKeyEvent>

addMon::addMon(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addMon)
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

        qry.prepare("select id, maker, model, inv, ser, spisan, comment, year from Monitor where id=:id;");
        qry.bindValue(":id",id1);
        qry.exec();
        qry.next();
        ui->lineEdit->setText(qry.value(1).toString());
        ui->lineEdit_2->setText(qry.value(2).toString());
        ui->lineEdit_3->setText(qry.value(3).toString());
        ui->lineEdit_4->setText(qry.value(4).toString());
        ui->lineEdit_5->setText(qry.value(6).toString());
        ui->lineEdit_6->setText(qry.value(7).toString());
        switch (qry.value(5).toInt()) {
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

addMon::~addMon()
{
    delete ui;
}

void addMon::on_pushButton_2_clicked()
{
    if(id1==0){

        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)||!(ui->lineEdit_3->text().length()>0)||!(ui->lineEdit_4->text().length()>0)){

            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery prov;
            prov.prepare("select inv from Monitor where inv = :inv;");
            prov.bindValue(":inv",ui->lineEdit_3->text());
            prov.exec();
            prov.next();


            if(ui->lineEdit_3->text()==prov.value(0).toString()){
                 if(ui->lineEdit_3->text()=="б/н"){

                     QSqlQuery qry2;
                     qry2.prepare("INSERT INTO Monitor (maker, model, inv, ser, spisan, comment, year) "
                                 "VALUES (:maker, :model, :inv, :ser, :spisan, :comment, :year);");
                     qry2.bindValue(":maker",ui->lineEdit->text());
                     qry2.bindValue(":model",ui->lineEdit_2->text());
                     qry2.bindValue(":inv",ui->lineEdit_3->text());
                     qry2.bindValue(":ser",ui->lineEdit_4->text());
                     qry2.bindValue(":comment",ui->lineEdit_5->text());
                     qry2.bindValue(":year",ui->lineEdit_6->text());
                     qry2.bindValue(":spisan",ui->comboBox->currentIndex());

                     if(qry2.exec()){
                         QSqlQuery del_log;
                         del_log.prepare("exec delLog :tbl, :link;");
                         del_log.bindValue(":tbl","Monitor");
                         del_log.bindValue(":link",id1);
                         del_log.exec();
                         MdiArea->closeActiveSubWindow();
                         emit Reselect();       }
                }else{

             QMessageBox::critical(this,"Error","Монитор с таким инвентарным номером уже существует");
            }
            }else {


    QSqlQuery qry2;
    qry2.prepare("INSERT INTO Monitor (maker, model, inv, ser, spisan, comment, year) "
                "VALUES (:maker, :model, :inv, :ser, :spisan, :comment, :year);");
    qry2.bindValue(":maker",ui->lineEdit->text());
    qry2.bindValue(":model",ui->lineEdit_2->text());
    qry2.bindValue(":inv",ui->lineEdit_3->text());
    qry2.bindValue(":ser",ui->lineEdit_4->text());
    qry2.bindValue(":comment",ui->lineEdit_5->text());
    qry2.bindValue(":year",ui->lineEdit_6->text());
    qry2.bindValue(":spisan",ui->comboBox->currentIndex());

    if(qry2.exec()){
        QSqlQuery del_log;
        del_log.prepare("exec delLog :tbl, :link;");
        del_log.bindValue(":tbl","Monitor");
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
        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)||!(ui->lineEdit_3->text().length()>0)||!(ui->lineEdit_4->text().length()>0)){
            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery prov;
            prov.prepare("select inv from Monitor where inv = :inv;");
            prov.bindValue(":inv",ui->lineEdit_3->text());
            prov.exec();
            prov.next();
            if(ui->lineEdit_3->text()==prov.value(0).toString() && ui->lineEdit_3->text()!=qry.value(3).toString()){
             QMessageBox::critical(this,"Error","Монитор с таким инвентарным номером уже существует");
            }else{

       QSqlQuery qryUp;
       qryUp.prepare("UPDATE Monitor SET maker=:maker, model=:model, inv=:inv, ser=:ser, spisan=:spisan, comment=:comment, year=:year WHERE id=:id;");
       qryUp.bindValue(":id",id1);
       qryUp.bindValue(":maker",ui->lineEdit->text());
       qryUp.bindValue(":model",ui->lineEdit_2->text());
       qryUp.bindValue(":inv",ui->lineEdit_3->text());
       qryUp.bindValue(":ser",ui->lineEdit_4->text());
       qryUp.bindValue(":comment",ui->lineEdit_5->text());
       qryUp.bindValue(":year",ui->lineEdit_6->text());
       qryUp.bindValue(":spisan",ui->comboBox->currentIndex());

       if(qryUp.exec()){
           QSqlQuery del_log;
           del_log.prepare("exec delLog :tbl, :link;");
           del_log.bindValue(":tbl","Monitor");
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

void addMon::on_pushButton_clicked()
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","Monitor");
    del_log.bindValue(":link",id1);
    del_log.exec();
     MdiArea->closeActiveSubWindow();

}

void addMon::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","Monitor");
    del_log.bindValue(":link",id1);
    del_log.exec();
    event->accept();
    MdiArea->activatePreviousSubWindow();
}

void addMon::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}
