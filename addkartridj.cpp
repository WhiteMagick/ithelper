#include "addkartridj.h"
#include "ui_addkartridj.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

AddKartridj::AddKartridj(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddKartridj)
{
    ui->setupUi(this);

    MdiAreaAddKar= md;
    id1 = id;
    model = new checkboxModel;

    ui->tableView->setModel(model);



    if(id1==0){
        setWindowTitle("Добавление записи");
        qry.prepare("select s.id, CONCAT(m.maker,' ',s.model) from SalaryPrn s "
                    "inner join SalaryMakerPRN m on s.maker = m.id ;");
        qry.exec();
        qry.next();
        model->setQuery(qry);
        model->setHeaderData(1, Qt::Horizontal, "Принтер");
        ui->tableView->setColumnWidth(0, 30);
        ui->tableView->setColumnWidth(1, 300);



    }else{
        setWindowTitle("Редактирование записи");
        QSqlQuery qry1;
        qry1.prepare("select id, name, res from SalaryKartridj where id = :id;");
        qry1.bindValue(":id",id1);
        qry1.exec();
        qry1.next();
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
        ui->lineEdit->setText(qry1.value(1).toString());
        ui->lineEdit_2->setText(QString::number(qry1.value(2).toInt()));

        qry2.prepare("select s.id, CONCAT(m.maker,' ',s.model) from SalaryPrn s "
                    "inner join SalaryMakerPRN m on s.maker = m.id;");
        qry2.exec();
        qry2.next();
        model->setQuery(qry2);
        model->setHeaderData(1, Qt::Horizontal, "Принтер");
        ui->tableView->setColumnWidth(0, 30);
        ui->tableView->setColumnWidth(1, 300);

        QSqlQuery setchk;
        setchk.prepare("select s.idPrn from SovmestimostKartPrn s "
                       "where idKart = :id;");
        setchk.bindValue(":id",id1);
        setchk.exec();
        while(setchk.next()){

            for (int i = 0; i<model->rowCount();i++ ) {
               if(model->data(model->index(i,0),Qt::DisplayRole).toInt()== setchk.value(0).toInt()){
                   model->setData(model->index(i,0),Qt::Checked,Qt::CheckStateRole);


               }
            }

        }
 ui->tableView->setFocus();

    }
}

AddKartridj::~AddKartridj()
{
    delete ui;
}

void AddKartridj::on_pushButton_save_clicked()
{

    if(id1==0){

        QSqlQuery insertSal;
        insertSal.prepare("INSERT INTO SalaryKartridj (name, res) VALUES (:name, :res);");
        insertSal.bindValue(":name",ui->lineEdit->text());
        insertSal.bindValue(":res",ui->lineEdit_2->text().toInt());
        if(insertSal.exec()){
            MdiAreaAddKar->closeActiveSubWindow();
            MdiAreaAddKar->activatePreviousSubWindow();
        }else{
            QMessageBox::critical(this,"Error",insertSal.lastError().text());
        }

    }else if (id1!=0){

        QSqlQuery delold;
        delold.prepare("select count(s.idPrn) from SovmestimostKartPrn s "
                               "where idKart = :id;");
        delold.bindValue(":id",id1);
        delold.exec();
        delold.next();
        if(delold.value(0).toInt()!=0){
            QSqlQuery del;
            del.prepare("delete from SovmestimostKartPrn where idKart = :id;");
            del.bindValue(":id",id1);
            del.exec();
        }else{

        }
    int c=0;
        for (int i = 0; i<model->rowCount();i++ ) {
           if(model->data(model->index(i,0),Qt::CheckStateRole)==2){

                QSqlQuery insertSovm;
                insertSovm.prepare("INSERT INTO SovmestimostKartPrn (idPrn, idKart) VALUES (:idPrn, :idKart);");
                insertSovm.bindValue(":idPrn",model->data(model->index(i,0),Qt::DisplayRole).toInt());
                insertSovm.bindValue(":idKart",id1);
                insertSovm.exec();
                        c=1;
                        }

                    }

    if(c==0){
        QSqlQuery del;
        del.prepare("delete from SovmestimostKartPrn where idKart = :id;");
        del.bindValue(":id",id1);
        del.exec();
        }
    QSqlQuery upd;
    upd.prepare("UPDATE SalaryKartridj SET name=:name, res=:res where id =:id;");
    upd.bindValue(":id",id1);
    upd.bindValue(":name",ui->lineEdit->text());
    upd.bindValue(":res",ui->lineEdit_2->text().toInt());

    if(!upd.exec()){
        QMessageBox::critical(this,"Error",upd.lastError().text());
    }

    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","SalaryKartridj");
    del_log.bindValue(":link",id1);
    del_log.exec();
     emit Reselect();
    MdiAreaAddKar->closeActiveSubWindow();
    MdiAreaAddKar->activatePreviousSubWindow();
    }

}

void AddKartridj::on_pushButton_exit_clicked()
{

    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","SalaryKartridj");
    del_log.bindValue(":link",id1);
    del_log.exec();
    MdiAreaAddKar->closeActiveSubWindow();
    MdiAreaAddKar->activatePreviousSubWindow();

}

void AddKartridj::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","SalaryKartridj");
    del_log.bindValue(":link",id1);
    del_log.exec();
    event->accept();
    MdiAreaAddKar->activatePreviousSubWindow();

}

void AddKartridj::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

      }

}
