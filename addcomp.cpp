#include "addcomp.h"
#include "os.h"
#include "ui_addcomp.h"
#include <QSqlError>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QScreen>
#include <QKeyEvent>

AddComp::AddComp(int id, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddComp)
{
    MdiAreaComp= md;
    id1=id;
    ui->setupUi(this);
    ui->comboBox->clear();
    ui->comboBox->addItem("Не списан",QVariant(0));
    ui->comboBox->addItem("Списан",QVariant(1));
    ui->comboBox->addItem("На списании",QVariant(2));
    //--------------------------------------------
    ui->comboBox_2->addItem("Нет");
    ui->comboBox_2->addItem("Есть");
   //---------------------------------------------------
    if(id1==0){
        setWindowTitle("Добавление записи");
        ui->lineEdit_6->setText("-");
        ui->lineEdit_7->setText("0");
        ui->lineEdit_8->setText("-");
        ui->lineEdit_10->setText("-");
    }
    else {
        setWindowTitle("Редактирование записи");

        qry.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan,blokhost, comment, year from Comp where id=:id;");
        qry.bindValue(":id",id1);
        qry.exec();
        qry.next();
        ui->lineEdit->setText(qry.value(1).toString());
        ui->lineEdit_2->setText(qry.value(2).toString());
        ui->lineEdit_3->setText(qry.value(3).toString());
        ui->lineEdit_4->setText(qry.value(4).toString());
        ui->lineEdit_5->setText(qry.value(5).toString());
        ui->lineEdit_6->setText(qry.value(6).toString());
        ui->lineEdit_7->setText(qry.value(7).toString());
        ui->lineEdit_8->setText(qry.value(8).toString());
        ui->lineEdit_9->setText(qry.value(9).toString());
        ui->lineEdit_10->setText(qry.value(10).toString());
        ui->lineEdit_11->setText(qry.value(11).toString());
        ui->lineEdit_12->setText(qry.value(12).toString());
        ui->lineEdit_13->setText(qry.value(13).toString());
        ui->lineEdit_14->setText(qry.value(14).toString());
        ui->lineEdit_16->setText(qry.value(17).toString());
        ui->lineEdit_15->setText(qry.value(18).toString());
        switch (qry.value(15).toInt()) {
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
        switch (qry.value(16).toInt()) {
        case 0:{
            ui->comboBox_2->setCurrentIndex(0);
            break;
        }
        case 1:{
            ui->comboBox_2->setCurrentIndex(1);
            break;
        }
        }




    }
}

AddComp::~AddComp()
{
    delete ui;
}

void AddComp::viborOS(int id1)
{

  QString s = QString::number(id1);
  ui->lineEdit_12->setText(s);

}

void AddComp::on_pushButton_clicked() //выбор
{
    OS *os = new OS(1,MdiAreaComp);
    QMdiSubWindow *subWindow = MdiAreaComp->addSubWindow(os);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 300, 300);
    subWindow->show();
  connect(os,SIGNAL(vibor(int)),this,SLOT(viborOS(int)));
}

void AddComp::on_btnSave_clicked()
{
    if(id1==0){

        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)||!(ui->lineEdit_3->text().length()>0)||!(ui->lineEdit_4->text().length()>0)
                ||!(ui->lineEdit_5->text().length()>0)||!(ui->lineEdit_6->text().length()>0)||!(ui->lineEdit_7->text().length()>0)||!(ui->lineEdit_8->text().length()>0)
                ||!(ui->lineEdit_9->text().length()>0)||!(ui->lineEdit_10->text().length()>0)||!(ui->lineEdit_11->text().length()>0)||!(ui->lineEdit_12->text().length()>0)){

            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery prov;
            prov.prepare("select inv from Comp where inv = :inv;");
            prov.bindValue(":inv",ui->lineEdit_4->text());
            prov.exec();
            prov.next();
            if(ui->lineEdit_4->text()==prov.value(0).toString()){
             QMessageBox::critical(this,"Error","Компьютер с таким инвентарным номером уже существует");
            }else {


    QSqlQuery qry2;
    qry2.prepare("INSERT INTO Comp (maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan,blokhost, comment, year) "
                "VALUES (:maker, :model, :name, :inv, :ser, :CPU, :cpuZ, :RAM, :ramZ, :HDD, :hddZ, :OSid, :eToken, :IP, :spisan,:blokhost, :comment, :year);");
    qry2.bindValue(":maker",ui->lineEdit->text());
    qry2.bindValue(":model",ui->lineEdit_2->text());
    qry2.bindValue(":name",ui->lineEdit_3->text());
    qry2.bindValue(":inv",ui->lineEdit_4->text());
    qry2.bindValue(":ser",ui->lineEdit_5->text());
    qry2.bindValue(":CPU",ui->lineEdit_6->text());
    qry2.bindValue(":cpuZ",ui->lineEdit_7->text());
    qry2.bindValue(":RAM",ui->lineEdit_8->text());
    qry2.bindValue(":ramZ",ui->lineEdit_9->text());
    qry2.bindValue(":HDD",ui->lineEdit_10->text());
    qry2.bindValue(":hddZ",ui->lineEdit_11->text());
    qry2.bindValue(":OSid",ui->lineEdit_12->text());
    qry2.bindValue(":eToken",ui->lineEdit_13->text());
    qry2.bindValue(":IP",ui->lineEdit_14->text());
    qry2.bindValue(":comment",ui->lineEdit_16->text());
    qry2.bindValue(":year",ui->lineEdit_15->text());
    qry2.bindValue(":spisan",ui->comboBox->currentIndex());
    qry2.bindValue(":blokhost",ui->comboBox_2->currentIndex());

    if(qry2.exec()){
        MdiAreaComp->closeActiveSubWindow();
        emit Reselect();                                        //Вызывает слот для обновления таблици на экране

    }else {
        QMessageBox::critical(this,"Error",qry2.lastError().text()+"Все поля являются обязательными для заполнения");
            }
           }
        }

    }else{
        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)||!(ui->lineEdit_3->text().length()>0)||!(ui->lineEdit_4->text().length()>0)
                ||!(ui->lineEdit_5->text().length()>0)||!(ui->lineEdit_6->text().length()>0)||!(ui->lineEdit_7->text().length()>0)||!(ui->lineEdit_8->text().length()>0)
                ||!(ui->lineEdit_9->text().length()>0)||!(ui->lineEdit_10->text().length()>0)||!(ui->lineEdit_11->text().length()>0)||!(ui->lineEdit_12->text().length()>0)){
            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{
            QSqlQuery prov;
            prov.prepare("select inv from Comp where inv = :inv;");
            prov.bindValue(":inv",ui->lineEdit_4->text());
            prov.exec();
            prov.next();
            if(ui->lineEdit_4->text()==prov.value(0).toString() && ui->lineEdit_4->text()!=qry.value(4).toString()){
             QMessageBox::critical(this,"Error","Компьютер с таким инвентарным номером уже существует");
            }else{

       QSqlQuery qryUp;
       qryUp.prepare("UPDATE Comp SET maker=:maker, model=:model, name=:name, inv=:inv, ser=:ser, CPU=:CPU, cpuZ=:cpuZ, RAM=:RAM, "
                     "ramZ=:ramZ, HDD=:HDD, hddZ=:hddZ, OSid=:OSid, eToken=:eToken, IP=:IP, spisan=:spisan,blokhost=:blokhost, comment=:comment, year=:year WHERE id=:id;");
       qryUp.bindValue(":id",id1);
       qryUp.bindValue(":maker",ui->lineEdit->text());
       qryUp.bindValue(":model",ui->lineEdit_2->text());
       qryUp.bindValue(":name",ui->lineEdit_3->text());
       qryUp.bindValue(":inv",ui->lineEdit_4->text());
       qryUp.bindValue(":ser",ui->lineEdit_5->text());
       qryUp.bindValue(":CPU",ui->lineEdit_6->text());
       qryUp.bindValue(":cpuZ",ui->lineEdit_7->text());
       qryUp.bindValue(":RAM",ui->lineEdit_8->text());
       qryUp.bindValue(":ramZ",ui->lineEdit_9->text());
       qryUp.bindValue(":HDD",ui->lineEdit_10->text());
       qryUp.bindValue(":hddZ",ui->lineEdit_11->text());
       qryUp.bindValue(":OSid",ui->lineEdit_12->text());
       qryUp.bindValue(":eToken",ui->lineEdit_13->text());
       qryUp.bindValue(":IP",ui->lineEdit_14->text());
       qryUp.bindValue(":comment",ui->lineEdit_16->text());
       qryUp.bindValue(":year",ui->lineEdit_15->text());
       qryUp.bindValue(":spisan",ui->comboBox->currentIndex());
       qryUp.bindValue(":blokhost",ui->comboBox_2->currentIndex());

       if(qryUp.exec()){

           QSqlQuery del_log;
           del_log.prepare("exec delLog :tbl, :link;");
           del_log.bindValue(":tbl","Comp");
           del_log.bindValue(":link",id1);
           del_log.exec();
           MdiAreaComp->closeActiveSubWindow();
           emit Reselect();                                        //Вызывает слот  для обновления таблици на экране

       }else {
           QMessageBox::critical(this,"Error",qryUp.lastError().text());
       }
}
    }
    }
}

void AddComp::on_pushButtonExit_clicked()
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","Comp");
    del_log.bindValue(":link",id1);
    del_log.exec();
    MdiAreaComp->closeActiveSubWindow();
}

void AddComp::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","Comp");
    del_log.bindValue(":link",id1);
    del_log.exec();
    event->accept();
    MdiAreaComp->activatePreviousSubWindow();

}

void AddComp::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}


