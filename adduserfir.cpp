#include "adduserfir.h"
#include "ui_adduserfir.h"
#include <QMessageBox>
#include <QSqlError>
addUserFir::addUserFir(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addUserFir)
{
    ui->setupUi(this);
    MdiArea= md;
    id1=id;
    ui->comboBox->clear();
    ui->comboBox->addItem("ХЗ",QVariant(0));
    ui->comboBox->addItem("Вкл",QVariant(1));
    ui->comboBox->addItem("Откл",QVariant(2));
    if(id1==0){
         setWindowTitle("Cоздание записи");
    }else{
    setWindowTitle("Редактирование записи");
    qry1.prepare("SELECT id, login, FIO, post, sub, d_reg, d_out, st, face_link from UserFir where id =:id;");
    qry1.bindValue(":id",id1);
    qry1.exec();
    qry1.next();
    ui->lineEdit->setText(qry1.value(1).toString());
    ui->lineEdit_2->setText(qry1.value(2).toString());
    ui->lineEdit_3->setText(qry1.value(4).toString());
    ui->lineEdit_4->setText(qry1.value(3).toString());
    ui->lineEdit_5->setText(qry1.value(8).toString());
    ui->dateEdit->setDate(qry1.value(5).toDate());
    if(qry1.value(6).toString() != NULL){
    ui->dateEdit_2->setDate(qry1.value(6).toDate());
    }else {
        }
    switch (qry1.value(7).toInt()) {
    case 0: {
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

addUserFir::~addUserFir()
{
    delete ui;
}

void addUserFir::on_pushButton_save_clicked()
{
    if(id1==0){
        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)||!(ui->lineEdit_5->text().length()>0)){
            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery insert;
            insert.prepare("INSERT INTO UserFir (login, FIO, post, sub, d_reg, d_out, st, face_link) VALUES (:login, :FIO, :post, :sub, :d_reg, :d_out, :st, :face_link);");
            insert.bindValue(":login",ui->lineEdit->text());
            insert.bindValue(":FIO",ui->lineEdit_2->text());
            if(ui->lineEdit_4->text().length()>0){
                insert.bindValue(":post",ui->lineEdit_4->text());
            }else{
                insert.bindValue(":post","");
            }
            if(ui->lineEdit_3->text().length()>0){
                insert.bindValue(":sub",ui->lineEdit_3->text());
            }else{
                insert.bindValue(":sub","");
            }
            insert.bindValue(":d_reg",ui->dateEdit->date().toString("dd.MM.yyyy"));
//            if(ui->dateEdit_2->date() == QDate(9999,12,31)){
//                insert.bindValue(":d_out","NULL");
//            }else{
            insert.bindValue(":d_out",ui->dateEdit_2->date().toString("dd.MM.yyyy"));
            //}
            insert.bindValue(":st",ui->comboBox->currentIndex());
            insert.bindValue(":face_link",ui->lineEdit_5->text());
            if(!insert.exec()){
               QMessageBox::critical(this,"Error",insert.lastError().text());
            }
            QSqlQuery del_log;
            del_log.prepare("exec delLog :tbl, :link;");
            del_log.bindValue(":tbl","UserFir");
            del_log.bindValue(":link",id1);
            del_log.exec();
            MdiArea->closeActiveSubWindow();
            emit Reselect();

        }
    }else{

        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)||!(ui->lineEdit_5->text().length()>0)){
            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery update;
            update.prepare("UPDATE UserFir SET login=:login, FIO=:FIO, post=:post, sub=:sub, d_reg=:d_reg, d_out=:d_out, st=:st, face_link=:face_link where id=:id;");
            update.bindValue(":login",ui->lineEdit->text());
            update.bindValue(":FIO",ui->lineEdit_2->text());
            if(ui->lineEdit_4->text().length()>0){
                update.bindValue(":post",ui->lineEdit_4->text());
            }else{
                update.bindValue(":post","");
            }
            if(ui->lineEdit_3->text().length()>0){
                update.bindValue(":sub",ui->lineEdit_3->text());
            }else{
                update.bindValue(":sub","");
            }
            update.bindValue(":d_reg",ui->dateEdit->date().toString("dd.MM.yyyy"));
            update.bindValue(":d_out",ui->dateEdit_2->date().toString("dd.MM.yyyy"));
            update.bindValue(":st",ui->comboBox->currentIndex());
            update.bindValue(":face_link",ui->lineEdit_5->text());
            update.bindValue(":id",id1);
            if(!update.exec()){
               QMessageBox::critical(this,"Error",update.lastError().text());
            }
            QSqlQuery del_log;
            del_log.prepare("exec delLog :tbl, :link;");
            del_log.bindValue(":tbl","UserFir");
            del_log.bindValue(":link",id1);
            del_log.exec();
            MdiArea->closeActiveSubWindow();
            emit Reselect();

        }

    }

}

void addUserFir::on_pushButton_close_clicked()
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","UserFir");
    del_log.bindValue(":link",id1);
    del_log.exec();
    MdiArea->closeActiveSubWindow();
}

void addUserFir::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }

}

void addUserFir::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","UserFir");
    del_log.bindValue(":link",id1);
    del_log.exec();
    MdiArea->closeActiveSubWindow();
}
