#include "addkolonki.h"
#include "ui_addkolonki.h"
#include <QSqlError>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QScreen>
#include <QKeyEvent>

AddKolonki::AddKolonki(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddKolonki)
{
    id1=id;
    MdiAreakol = md;
    ui->setupUi(this);
    if(id1==0){
        setWindowTitle("Добавление записи");

    }
    else {
        setWindowTitle("Редактирование записи");
        qry.prepare("select id, GUID, Kab, How_use, Kolonki, Instal, Date_in, Date_out, Comment, Sost from Kolonky2 where id=:id;");
        qry.bindValue(":id",id1);
        qry.exec();
        qry.next();
        ui->lineEdit_kab->setText(qry.value(2).toString());
        ui->lineEdit_user->setText(qry.value(3).toString());
        ui->lineEdit_kol->setText(qry.value(4).toString());
        ui->lineEdit_inst->setText(qry.value(5).toString());
        ui->dateEdit_in->setDate(qry.value(6).toDate());
        ui->dateEdit_out->setDate(qry.value(7).toDate());
        ui->lineEdit_komm->setText(qry.value(8).toString());
        switch (qry.value(9).toInt()) {
        case 0:{
                ui->checkBox->setChecked(false);
                st=0;
                break;
                }
        case 1:{
                ui->checkBox->setChecked(true);
                st=1;
                break;
                }
        }
    }
}

AddKolonki::~AddKolonki()
{
    delete ui;
}

void AddKolonki::on_pushButton_2_clicked() //save
{
    if(id1==0){ //insert
        if(!(ui->lineEdit_kab->text().length()>0) || !(ui->lineEdit_kol->text().length()>0) || !(ui->lineEdit_inst->text().length()>0) || !(ui->lineEdit_user->text().length()>0)){
            QMessageBox::critical(this,"Error","Заполните обязательные поля");

        }else{
            if(ui->checkBox->isChecked()){
                st=1;
            }else{
                st=0;
            }
            QSqlQuery insert;
            insert.prepare("INSERT INTO Kolonky2 (Kab, How_use, Kolonki, Instal, Date_in, Date_out, Comment, Sost) VALUES (:Kab, :How_use, :Kolonki, :Instal, :Date_in, :Date_out, :Comment, :Sost);");
            insert.bindValue(":Kab",ui->lineEdit_kab->text());
            insert.bindValue(":How_use", ui->lineEdit_user->text());
            insert.bindValue(":Kolonki", ui->lineEdit_kol->text());
            insert.bindValue(":Instal", ui->lineEdit_inst->text());
            insert.bindValue(":Date_in",ui->dateEdit_in->date().toString("dd.MM.yyyy"));
            insert.bindValue(":Date_out",ui->dateEdit_out->date().toString("dd.MM.yyyy"));
            insert.bindValue(":Comment", ui->lineEdit_komm->text());
            insert.bindValue(":Sost",st);
            if(insert.exec()){
               MdiAreakol->closeActiveSubWindow();
               emit Reselect();
            }else{
                QMessageBox::critical(this,"Error",insert.lastError().text()+"Все поля являются обязательными для заполнения");
            }

        }

    }else{//update
        if(!(ui->lineEdit_kab->text().length()>0) || !(ui->lineEdit_kol->text().length()>0) || !(ui->lineEdit_inst->text().length()>0) || !(ui->lineEdit_user->text().length()>0)){
        QMessageBox::critical(this,"Error","Заполните обязательные поля");

}else{
            if(ui->checkBox->isChecked()){
                st=1;
            }else{
                st=0;
            }
            QSqlQuery update;
        update.prepare("UPDATE Kolonky2 SET Kab=:Kab, How_use=:How_use, Kolonki=:Kolonki, Instal=:Instal, Date_in=:Date_in, Date_out=:Date_out, Comment=:Comment, Sost=:Sost where id=:id");
        update.bindValue(":id",id1);
        update.bindValue(":Kab",ui->lineEdit_kab->text());
        update.bindValue(":How_use", ui->lineEdit_user->text());
        update.bindValue(":Kolonki", ui->lineEdit_kol->text());
        update.bindValue(":Instal", ui->lineEdit_inst->text());
        update.bindValue(":Date_in",ui->dateEdit_in->date().toString("dd.MM.yyyy"));
        update.bindValue(":Date_out",ui->dateEdit_out->date().toString("dd.MM.yyyy"));
        update.bindValue(":Comment", ui->lineEdit_komm->text());
        update.bindValue(":Sost",st);
        if(update.exec()){
            QSqlQuery del_log;
            del_log.prepare("exec delLog :tbl, :link;");
            del_log.bindValue(":tbl","Kolonky2");
            del_log.bindValue(":link",id1);
            del_log.exec();
            MdiAreakol->closeActiveSubWindow();
            emit Reselect();                                        //Вызывает слот  для обновления таблици на экране

       }else{
            QMessageBox::critical(this,"Error",update.lastError().text());
        }


       }
    }

}

void AddKolonki::on_pushButton_clicked()
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","Kolonky2");
    del_log.bindValue(":link",id1);
    del_log.exec();
    MdiAreakol->closeActiveSubWindow();
}

void AddKolonki::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","Kolonky2");
    del_log.bindValue(":link",id1);
    del_log.exec();
    event->accept();
    MdiAreakol->activatePreviousSubWindow();
}

void AddKolonki::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}
