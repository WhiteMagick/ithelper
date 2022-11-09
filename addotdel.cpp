#include "addotdel.h"
#include "ui_addotdel.h"
#include <QSqlError>
#include <QCloseEvent>
#include <QKeyEvent>

AddOtdel::AddOtdel(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOtdel)
{
    MdiArea = md;
    id1=id;
    ui->setupUi(this);
    if(id1==0){
        setWindowTitle("Добавление записи");

    }else {
    setWindowTitle("Редактирование записи");

    qry1.prepare("select id, name, CODE from Otdel where id=:id;");
    qry1.bindValue(":id",id1);
    qry1.exec();
    qry1.next();
    ui->lineEdit->setText(qry1.value(2).toString());
    ui->lineEdit_2->setText(qry1.value(1).toString());
}
}

AddOtdel::~AddOtdel()
{
    delete ui;
}

void AddOtdel::on_pushButton_2_clicked()
{
    if(id1==0){

        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)){
            QMessageBox::critical(this,"Error","Все поля являются обязательными для заполнения");
        }else{
            QSqlQuery cod;                                                              //проверка на уникальность кода
            cod.prepare("select CODE from Otdel where CODE=:id;");
            cod.bindValue(":id",ui->lineEdit->text());
            cod.exec();
            cod.next();
            QString code=cod.value(0).toString();

            if(code!=ui->lineEdit->text()){

    QSqlQuery qry;
    qry.prepare("INSERT INTO Otdel (CODE, name) VALUES (:cod,:name);");
    qry.bindValue(":cod",ui->lineEdit->text());
    qry.bindValue(":name",ui->lineEdit_2->text());


    if(qry.exec()){

        MdiArea->closeActiveSubWindow();      
        emit Reselect();                                        //Вызывает слот для обновления таблици на экране

    }else {
        QMessageBox::critical(this,"Error",qry.lastError().text()+"Все поля являются обязательными для заполнения");
    }

            }else{
              QMessageBox::critical(this,"Error","Отдел с таким кодом уже существует");
            }
            }
    }else{
        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)){
            QMessageBox::critical(this,"Error","Все поля являются обязательными для заполнения");
        }else{
            QSqlQuery cod;                                                              //проверка на уникальность кода
            cod.prepare("select CODE from Otdel where CODE=:id;");
            cod.bindValue(":id",ui->lineEdit->text());
            cod.exec();
            cod.next();
            QString code=cod.value(0).toString();
            QString code1=qry1.value(2).toString();

            if(code!=ui->lineEdit->text() || code==code1){          // проверка на повтор кода с искючением того что было

       QSqlQuery qryUp;
       qryUp.prepare("UPDATE Otdel SET CODE=:cod, name=:name WHERE id=:id;");
       qryUp.bindValue(":id",id1);
       qryUp.bindValue(":cod",ui->lineEdit->text());
       qryUp.bindValue(":name",ui->lineEdit_2->text());
       if(qryUp.exec()){

           MdiArea->closeActiveSubWindow();

           emit Reselect();                                        //Вызывает слот  для обновления таблици на экране

       }else {
           QMessageBox::critical(this,"Error",qryUp.lastError().text()+"Все поля являются обязательными для заполнения");
       }
            }else{
              QMessageBox::critical(this,"Error","Отдел с таким кодом уже существует");
            }
    }
    }
}

void AddOtdel::on_pushButton_clicked()
{
    MdiArea->closeActiveSubWindow();
}

void AddOtdel::closeEvent(QCloseEvent *event)
{
    event->accept();
    MdiArea->activatePreviousSubWindow();

}

void AddOtdel::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }

}
