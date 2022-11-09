#include "addperepis.h"
#include "ui_addperepis.h"
#include "search.h"
#include "addcomp.h"
#include "addmon.h"
#include <QSqlError>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QScreen>
#include <QKeyEvent>

addPerepis::addPerepis(int id, int idOT, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPerepis)
{
    MdiAreaP = md;
    MdiAreaP->setOption(QMdiArea::DontMaximizeSubWindowOnActivation, true);
    idOtdel= idOT;
    id1=id;
    ui->setupUi(this);

    statusList = {"Не списан","Списан","На списании"};
    statusBH = {"Есть","Нет"};


    if(id1==0){
        setWindowTitle("Добавление записи");
    }
    else {
        setWindowTitle("Редактирование записи");

        fill();
    }
}

addPerepis::~addPerepis()
{
    delete ui;
}

void addPerepis::viborPC(int id1)
{

    viborPc= id1;
    qryP.prepare("select c.id, CONCAT(c.maker,' ',c.model,' ') as 'Компьютер', c.name, c.inv, c.ser, c.CPU, c.cpuZ, c.RAM, c.ramZ, c.HDD, c.hddZ, c.OSid, c.eToken, c.IP, c.spisan, c.blokhost,CONCAT(o.name,' ',o.razr,'x') as 'операционная система' from Comp c "
                 "inner join OS o on o.id = c.OSid"
                 " where c.id = :id;");
    qryP.bindValue(":id",viborPc);
    qryP.exec();
    qryP.next();
    ui->lineEdit_4->setText(qryP.value(1).toString());
    ui->lineEdit_5->setText(qryP.value(2).toString());
    ui->lineEdit_6->setText(qryP.value(3).toString());
    ui->lineEdit_7->setText(qryP.value(4).toString());
    ui->lineEdit_15->setText(qryP.value(12).toString());
    ui->lineEdit_16->setText(qryP.value(16).toString());
    switch (qryP.value(14).toInt()) {
    case 0:{
        ui->label_18->setText(statusList[0]);
        break;
    }
    case 1:{
        ui->label_18->setText(statusList[1]);
        break;
    }
    case 2:{
        ui->label_18->setText(statusList[2]);
        break;
    }
    }
    switch (qryP.value(15).toInt()) {
    case 0:{
        ui->label_BH_st->setText(statusBH[1]);
        break;
    }
    case 1:{
        ui->label_BH_st->setText(statusBH[0]);
        break;
    }
    }


}

void addPerepis::viborMON(int id1)
{
 viborMon= id1;
 qryM.prepare("select id, CONCAT(maker,' ',model,' ') as 'Монитор', inv, ser, spisan, comment from Monitor where id = :id;");
 qryM.bindValue(":id",viborMon);
 qryM.exec();
 qryM.next();
 ui->lineEdit_9->setText(qryM.value(1).toString());
 ui->lineEdit_10->setText(qryM.value(2).toString());
 ui->lineEdit_11->setText(qryM.value(3).toString());
 switch (qryM.value(4).toInt()) {
 case 0:{
     ui->label_MON_st->setText(statusList[0]);
     break;
 }
 case 1:{
     ui->label_MON_st->setText(statusList[1]);
     break;
 }
 case 2:{
     ui->label_MON_st->setText(statusList[2]);
     break;
 }
 }


}

void addPerepis::on_viborPC_clicked()
{
    Search *poisk = new Search(MdiAreaP,0,"");
    QMdiSubWindow *subWindow = MdiAreaP->addSubWindow(poisk);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 350);
    subWindow->show();
    connect(poisk,SIGNAL(Vibor(int)),this,SLOT(viborPC(int)));
}

void addPerepis::on_viborMon_clicked()
{
    Search *poisk = new Search(MdiAreaP,1,"");
    QMdiSubWindow *subWindow = MdiAreaP->addSubWindow(poisk);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 350);
    subWindow->show();
    connect(poisk,SIGNAL(Vibor(int)),this,SLOT(viborMON(int)));
}

void addPerepis::on_save_clicked()
{
    if(id1==0){

        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)||!(ui->lineEdit_3->text().length()>0)||!(ui->lineEdit_6->text().length()>0)
                ||!(ui->lineEdit_10->text().length()>0)){

            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{

    QSqlQuery qry2;
    qry2.prepare("INSERT INTO perepis ( OTDELID, KAB, USERNAM, USERUZ, COMPID, MONITORID, ROZPC, TEL, ROZTEL, COMMENT) "
                "VALUES (:OTDELID, :KAB, :USERNAM, :USERUZ, :COMPID, :MONITORID, :ROZPC, :TEL, :ROZTEL, :COMMENT);");
    qry2.bindValue(":OTDELID",idOtdel);
    qry2.bindValue(":KAB",ui->lineEdit->text());
    qry2.bindValue(":USERNAM",ui->lineEdit_2->text());
    qry2.bindValue(":USERUZ",ui->lineEdit_3->text());
    qry2.bindValue(":COMPID",viborPc);
    qry2.bindValue(":MONITORID",viborMon);
    qry2.bindValue(":ROZPC",ui->lineEdit_8->text());
    qry2.bindValue(":TEL",ui->lineEdit_12->text());
    qry2.bindValue(":ROZTEL",ui->lineEdit_13->text());
    qry2.bindValue(":COMMENT",ui->lineEdit_14->text());
    if(qry2.exec()){
        MdiAreaP->closeActiveSubWindow();

        emit Reselect();                                        //Вызывает слот для обновления таблици на экране
    }else {
        QMessageBox::critical(this,"Error",qry2.lastError().text());
            }

        }

    }else{
        if(!(ui->lineEdit->text().length()>0)||!(ui->lineEdit_2->text().length()>0)||!(ui->lineEdit_3->text().length()>0)||!(ui->lineEdit_6->text().length()>0)
                ||!(ui->lineEdit_10->text().length()>0)){
            QMessageBox::critical(this,"Error","Заполните обязательные поля");
        }else{


       QSqlQuery qryUp;
       qryUp.prepare("UPDATE perepis SET OTDELID=:OTDELID, KAB=:KAB, USERNAM=:USERNAM, USERUZ=:USERUZ, COMPID=:COMPID, MONITORID=:MONITORID, ROZPC=:ROZPC, TEL=:TEL, ROZTEL=:ROZTEL, COMMENT=:COMMENT WHERE id=:id;");
       qryUp.bindValue(":id",id1);
       qryUp.bindValue(":OTDELID",idOtdel);
       qryUp.bindValue(":KAB",ui->lineEdit->text());
       qryUp.bindValue(":USERNAM",ui->lineEdit_2->text());
       qryUp.bindValue(":USERUZ",ui->lineEdit_3->text());
       qryUp.bindValue(":COMPID",viborPc);
       qryUp.bindValue(":MONITORID",viborMon);
       qryUp.bindValue(":ROZPC",ui->lineEdit_8->text());
       qryUp.bindValue(":TEL",ui->lineEdit_12->text());
       qryUp.bindValue(":ROZTEL",ui->lineEdit_13->text());
       qryUp.bindValue(":COMMENT",ui->lineEdit_14->text());

       if(qryUp.exec()){

           QSqlQuery del_log;
           del_log.prepare("exec delLog :tbl, :link;");
           del_log.bindValue(":tbl","perepis");
           del_log.bindValue(":link",id1);
           del_log.exec();
           MdiAreaP->closeActiveSubWindow();
           //MdiAreaP->activatePreviousSubWindow();
           emit Reselect();                                        //Вызывает слот  для обновления таблици на экране

       }else {
           QMessageBox::critical(this,"Error",qryUp.lastError().text());
       }
}

    }


}

void addPerepis::on_exit_clicked()
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","perepis");
    del_log.bindValue(":link",id1);
    del_log.exec();

    MdiAreaP->closeActiveSubWindow();
    //MdiAreaP->activatePreviousSubWindow();
}

void addPerepis::on_viewPK_clicked()
{
QSqlQuery red;
red.prepare("select COMPID from perepis where id = :id;");
red.bindValue(":id",id1);
red.exec();
red.next();
AddComp *comp = new AddComp(red.value(0).toInt(),MdiAreaP);
QMdiSubWindow *subWindow = MdiAreaP->addSubWindow(comp);
QScreen *screen = qApp->primaryScreen();
int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
subWindow->setGeometry(x, y, 1031, 340);
subWindow->show();
 connect(comp,SIGNAL(Reselect()),this,SLOT(fill()));

}

void addPerepis::on_viewMon_clicked()
{
    QSqlQuery red;
    red.prepare("select MONITORID from perepis where id = :id;");
    red.bindValue(":id",id1);
    red.exec();
    red.next();
    addMon *mon = new addMon(red.value(0).toInt(),MdiAreaP);
    QMdiSubWindow *subWindow = MdiAreaP->addSubWindow(mon);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 627, 300);
    subWindow->show();
     connect(mon,SIGNAL(Reselect()),this,SLOT(fill()));

}

void addPerepis::fill()
{
    qry.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер',"
                "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий',"
                "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') from perepis p "
                "left join Comp c on c.id = p.COMPID "
                "left join Monitor m on m.id = p.MONITORID "
                "left join Otdel ot on ot.CODE = p.OTDELID "
                "left join OS o on o.id = c.OSid "
                "where p.id = :id;");
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
    ui->lineEdit_8->setText(qry.value(11).toString());
    ui->lineEdit_9->setText(qry.value(8).toString());
    ui->lineEdit_10->setText(qry.value(9).toString());
    ui->lineEdit_11->setText(qry.value(10).toString());
    ui->lineEdit_12->setText(qry.value(12).toString());
    ui->lineEdit_13->setText(qry.value(13).toString());
    ui->lineEdit_14->setText(qry.value(14).toString());
    ui->lineEdit_15->setText(qry.value(18).toString());
    ui->lineEdit_16->setText(qry.value(19).toString());
    switch (qry.value(15).toInt()) {
    case 0:{
        ui->label_18->setText(statusList[0]);
        break;
    }
    case 1:{
        ui->label_18->setText(statusList[1]);
        break;
    }
    case 2:{
        ui->label_18->setText(statusList[2]);
        break;
    }
    }
    switch (qry.value(16).toInt()) {
    case 0:{
        qDebug()<<"fiil";

        ui->label_BH_st->setText(statusBH[1]);
        break;
    }
    case 1:{
        ui->label_BH_st->setText(statusBH[0]);
        break;
    }
    }

    switch (qry.value(17).toInt()) {
    case 0:{
        ui->label_MON_st->setText(statusList[0]);
        break;
    }
    case 1:{
        ui->label_MON_st->setText(statusList[1]);
        break;
    }
    case 2:{
        ui->label_MON_st->setText(statusList[2]);
        break;
    }

    }


ui->lineEdit_4->setReadOnly(true);
ui->lineEdit_5->setReadOnly(true);
ui->lineEdit_6->setReadOnly(true);
ui->lineEdit_7->setReadOnly(true);
ui->lineEdit_9->setReadOnly(true);
ui->lineEdit_10->setReadOnly(true);
ui->lineEdit_11->setReadOnly(true);
ui->lineEdit_15->setReadOnly(true);

QSqlQuery idPC;
idPC.prepare("select id from Comp where inv = :inv;");
idPC.bindValue(":inv",ui->lineEdit_6->text());
idPC.exec();
idPC.next();
viborPc=idPC.value(0).toInt();

QSqlQuery idMon;
idMon.prepare("select id from Monitor where inv = :inv;");
idMon.bindValue(":inv",ui->lineEdit_10->text());
idMon.exec();
idMon.next();
viborMon=idMon.value(0).toInt();

}

void addPerepis::closeEvent(QCloseEvent *event)
{
    QSqlQuery del_log;
    del_log.prepare("exec delLog :tbl, :link;");
    del_log.bindValue(":tbl","perepis");
    del_log.bindValue(":link",id1);
    del_log.exec();
    event->accept();
    MdiAreaP->activatePreviousSubWindow();
}

void addPerepis::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}
