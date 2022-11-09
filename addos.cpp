#include "addos.h"
#include "ui_addos.h"
#include <QSqlError>
#include <QCloseEvent>
#include <QKeyEvent>

AddOs::AddOs(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOs)
{
    MdiArea=md;
    id1=id;
    ui->setupUi(this);

    if(id1==0){
        setWindowTitle("Добавление записи");

    }else {
    setWindowTitle("Редактирование записи");

    qry1.prepare("select  id, name, razr from OS where id=:id;");
    qry1.bindValue(":id",id1);
    qry1.exec();
    qry1.next();
    ui->lineEdit->setText(qry1.value(1).toString());
    ui->lineEdit_2->setText(qry1.value(2).toString());
}
}

AddOs::~AddOs()
{
    delete ui;
}

void AddOs::on_pushButton_2_clicked()
{
    if(id1==0){

        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)){
            QMessageBox::critical(this,"Error","Все поля являются обязательными для заполнения");
        }else{
    QSqlQuery qry;
    qry.prepare("INSERT INTO OS (name,razr) VALUES (:name,:razr);");
    qry.bindValue(":name",ui->lineEdit->text());
    qry.bindValue(":razr",ui->lineEdit_2->text());
    if(qry.exec()){
        MdiArea->closeActiveSubWindow();
        emit Reselect();                                        //Вызывает слот для обновления таблици на экране

    }else {
        QMessageBox::critical(this,"Error",qry.lastError().text()+"Все поля являются обязательными для заполнения");
    }
            }
    }else{
        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)){
            QMessageBox::critical(this,"Error","Все поля являются обязательными для заполнения");
        }else{
       QSqlQuery qryUp;
       qryUp.prepare("UPDATE OS SET name=:name,razr=:razr WHERE id=:id;");
       qryUp.bindValue(":id",id1);
       qryUp.bindValue(":name",ui->lineEdit->text());
       qryUp.bindValue(":razr",ui->lineEdit_2->text());
       if(qryUp.exec()){

           MdiArea->closeActiveSubWindow();

           emit Reselect();                                        //Вызывает слот  для обновления таблици на экране

       }else {
           QMessageBox::critical(this,"Error",qryUp.lastError().text()+"Все поля являются обязательными для заполнения");
       }

    }
    }
}

void AddOs::on_pushButton_clicked()
{
MdiArea->closeActiveSubWindow();

}

void AddOs::closeEvent(QCloseEvent *event)
{
    event->accept();
    MdiArea->activatePreviousSubWindow();
}

void AddOs::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}
