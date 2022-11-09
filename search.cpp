#include "search.h"
#include "ui_search.h"
#include <QKeyEvent>

Search::Search(QMdiArea *md, int id, QString g, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    GUID = g;
    MdiAreaSearch= md;
    id1=id;
    ui->setupUi(this);
    Login *k=new Login;

    qmodel = new QSqlTableModel(this,k->db);
    if(id1==0){
        setWindowTitle("Поиск");
        pc.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, comment from Comp с where not exists (select p.COMPID from perepis p where p.COMPID=с.id)or с.maker like 'Нет пк';");
        pc.exec();
        pc.next();
       qmodel->setQuery(pc);
       ui->tableView->setModel(qmodel);
        //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    }
    else if (id1==1) {
        setWindowTitle("Поиск");
        mon.prepare("select m.id,m.inv,m.maker,m.model,m.ser,m.spisan,m.comment from Monitor m where not exists (select p.MONITORID from perepis p where p.MONITORID=m.id) or m.maker like 'нет монитора';");
        mon.exec();
        mon.next();
        qmodel->setQuery(mon);
        ui->tableView->setModel(qmodel);
        //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    }
    else if(id1==2){
        setWindowTitle("История записи");
        ui->label->hide();
        ui->lineEdit->hide();
        ui->pushButton->hide();
        kol.prepare("select Data_mod, USER_mod, GUID, Kab, How_use, Kolonki, Instal, Date_in, Date_out, Comment FROM Kolonki_history where GUID = :g;");
        kol.bindValue(":g",GUID);
        kol.exec();
        kol.next();
        qmodel->setQuery(kol);
         qmodel->setHeaderData(0, Qt::Horizontal, "Дата модификации");
        qmodel->setHeaderData(1, Qt::Horizontal, "Изменил");
        qmodel->setHeaderData(2, Qt::Horizontal, "GUID");
          qmodel->setHeaderData(3, Qt::Horizontal, "Кабинет");
           qmodel->setHeaderData(4, Qt::Horizontal, "Пользователь");
         qmodel->setHeaderData(5, Qt::Horizontal, "Колонки");
         qmodel->setHeaderData(6, Qt::Horizontal, "Кто установил");
        qmodel->setHeaderData(7, Qt::Horizontal, "Дата установки");
        qmodel->setHeaderData(8, Qt::Horizontal, "Дата возврата");
        qmodel->setHeaderData(9, Qt::Horizontal, "Коментарий");

        ui->tableView->setModel(qmodel);
        ui->tableView->setColumnWidth(0, 150);
        ui->tableView->setColumnWidth(1, 250);
        ui->tableView->setColumnWidth(2, 400);
        ui->tableView->setColumnWidth(3, 80);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->setColumnWidth(5, 250);
        ui->tableView->setColumnWidth(6, 200);
        ui->tableView->setColumnWidth(7, 120);
        ui->tableView->setColumnWidth(8, 120);
        ui->tableView->setColumnWidth(9, 200);


    }
    else if (id1==3) {
        if(GUID.length()>0){
            setWindowTitle("История записи");
            ui->label->hide();
            ui->pushButton->hide();
            ui->lineEdit->hide();
            per.prepare("SELECT id, Data_mod, USER_mod, action, id_zap, OTDELID, KAB, USERNAM, USERUZ, COMPID,"
                        " MONITORID, ROZPC, TEL, ROZTEL, COMMENT, STATUS FROM  perepis_history where id_zap = :id;");
            per.bindValue(":id",GUID);
            per.exec();
            per.next();
            qmodel->setQuery(per);
            qmodel->setHeaderData(0, Qt::Horizontal, "ИД");
            qmodel->setHeaderData(1, Qt::Horizontal, "Дата модификации");
            qmodel->setHeaderData(2, Qt::Horizontal, "Изменил");
            qmodel->setHeaderData(3, Qt::Horizontal, "Действие");
            qmodel->setHeaderData(4, Qt::Horizontal, "ИД изм. записи");
            qmodel->setHeaderData(5, Qt::Horizontal, "ИД отдела");
            qmodel->setHeaderData(6, Qt::Horizontal, "Кабинет");
            qmodel->setHeaderData(7, Qt::Horizontal, "ФИО");
            qmodel->setHeaderData(8, Qt::Horizontal, "УЗ");
            qmodel->setHeaderData(9, Qt::Horizontal, "ИД ПК");
            qmodel->setHeaderData(10, Qt::Horizontal, "ИД мон.");
            qmodel->setHeaderData(11, Qt::Horizontal, "Розетка ПК");
            qmodel->setHeaderData(12, Qt::Horizontal, "Телефон");
            qmodel->setHeaderData(13, Qt::Horizontal, "Розетка тел.");
            qmodel->setHeaderData(14, Qt::Horizontal, "Комментарий");
            qmodel->setHeaderData(15, Qt::Horizontal, "Статус");
            ui->tableView->setModel(qmodel);
            ui->tableView->setColumnWidth(0, 50);
            ui->tableView->setColumnWidth(1, 200);
            ui->tableView->setColumnWidth(2, 200);
            ui->tableView->setColumnWidth(3, 500);
            ui->tableView->setColumnWidth(4, 80);
            ui->tableView->setColumnWidth(5, 80);
            ui->tableView->setColumnWidth(6, 80);
            ui->tableView->setColumnWidth(7, 250);
            ui->tableView->setColumnWidth(8, 180);
            ui->tableView->setColumnWidth(9, 80);
            ui->tableView->setColumnWidth(10, 80);
            ui->tableView->setColumnWidth(11, 80);
            ui->tableView->setColumnWidth(12, 80);
            ui->tableView->setColumnWidth(13, 80);
            ui->tableView->setColumnWidth(14, 250);
            ui->tableView->setColumnWidth(15, 80);

        }
        else {
            setWindowTitle("История переписи Компьютеров");
            ui->label->hide();
            ui->pushButton->hide();
            ui->lineEdit->hide();
            per.prepare("SELECT id, Data_mod, USER_mod, action, id_zap, OTDELID, KAB, USERNAM, USERUZ, COMPID,"
                        " MONITORID, ROZPC, TEL, ROZTEL, COMMENT, STATUS FROM  perepis_history;");
            per.exec();
            per.next();
            qmodel->setQuery(per);
            qmodel->setHeaderData(0, Qt::Horizontal, "ИД");
            qmodel->setHeaderData(1, Qt::Horizontal, "Дата модификации");
            qmodel->setHeaderData(2, Qt::Horizontal, "Изменил");
            qmodel->setHeaderData(3, Qt::Horizontal, "Действие");
            qmodel->setHeaderData(4, Qt::Horizontal, "ИД изм. записи");
            qmodel->setHeaderData(5, Qt::Horizontal, "ИД отдела");
            qmodel->setHeaderData(6, Qt::Horizontal, "Кабинет");
            qmodel->setHeaderData(7, Qt::Horizontal, "ФИО");
            qmodel->setHeaderData(8, Qt::Horizontal, "УЗ");
            qmodel->setHeaderData(9, Qt::Horizontal, "ИД ПК");
            qmodel->setHeaderData(10, Qt::Horizontal, "ИД мон.");
            qmodel->setHeaderData(11, Qt::Horizontal, "Розетка ПК");
            qmodel->setHeaderData(12, Qt::Horizontal, "Телефон");
            qmodel->setHeaderData(13, Qt::Horizontal, "Розетка тел.");
            qmodel->setHeaderData(14, Qt::Horizontal, "Комментарий");
            qmodel->setHeaderData(15, Qt::Horizontal, "Статус");

        ui->tableView->setModel(qmodel);
        ui->tableView->setColumnWidth(0, 50);
        ui->tableView->setColumnWidth(1, 200);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 500);
        ui->tableView->setColumnWidth(4, 80);
        ui->tableView->setColumnWidth(5, 80);
        ui->tableView->setColumnWidth(6, 80);
        ui->tableView->setColumnWidth(7, 250);
        ui->tableView->setColumnWidth(8, 180);
        ui->tableView->setColumnWidth(9, 80);
        ui->tableView->setColumnWidth(10, 80);
        ui->tableView->setColumnWidth(11, 80);
        ui->tableView->setColumnWidth(12, 80);
        ui->tableView->setColumnWidth(13, 80);
        ui->tableView->setColumnWidth(14, 250);
        ui->tableView->setColumnWidth(15, 80);

}


    }
    else if (id1==4) {
        if(GUID.length()>0){
            setWindowTitle("История записи");
            ui->label->hide();
            ui->pushButton->hide();
            ui->lineEdit->hide();
            per.prepare("SELECT id, Data_mod, USER_mod,action,id_zap,OTDELID, kab, model, name, IP, "
                        "inv,ser,roz,comment,spisan,STATUS FROM printer_history where id_zap = :id;");
            per.bindValue(":id",GUID);
            per.exec();
            per.next();
            qmodel->setQuery(per);
            qmodel->setHeaderData(0, Qt::Horizontal, "ИД");
            qmodel->setHeaderData(1, Qt::Horizontal, "Дата модификации");
            qmodel->setHeaderData(2, Qt::Horizontal, "Изменил");
            qmodel->setHeaderData(3, Qt::Horizontal, "Действие");
            qmodel->setHeaderData(4, Qt::Horizontal, "ИД изм. записи");
            qmodel->setHeaderData(5, Qt::Horizontal, "ИД отдела");
            qmodel->setHeaderData(6, Qt::Horizontal, "Кабинет");
            qmodel->setHeaderData(7, Qt::Horizontal, "Модель");
            qmodel->setHeaderData(8, Qt::Horizontal, "Имя");
            qmodel->setHeaderData(9, Qt::Horizontal, "IP");
            qmodel->setHeaderData(10, Qt::Horizontal, "Инвентарный");
            qmodel->setHeaderData(11, Qt::Horizontal, "Серийный");
            qmodel->setHeaderData(12, Qt::Horizontal, "Розетка");
            qmodel->setHeaderData(13, Qt::Horizontal, "Комментарий");
            qmodel->setHeaderData(14, Qt::Horizontal, "Статус списания");
            qmodel->setHeaderData(15, Qt::Horizontal, "Статус");
            ui->tableView->setModel(qmodel);
            ui->tableView->setColumnWidth(0, 50);
            ui->tableView->setColumnWidth(1, 200);
            ui->tableView->setColumnWidth(2, 200);
            ui->tableView->setColumnWidth(3, 500);
            ui->tableView->setColumnWidth(4, 80);
            ui->tableView->setColumnWidth(5, 80);
            ui->tableView->setColumnWidth(6, 80);
            ui->tableView->setColumnWidth(7, 250);
            ui->tableView->setColumnWidth(8, 180);
            ui->tableView->setColumnWidth(9, 80);
            ui->tableView->setColumnWidth(10, 80);
            ui->tableView->setColumnWidth(11, 80);
            ui->tableView->setColumnWidth(12, 80);
            ui->tableView->setColumnWidth(13, 80);
            ui->tableView->setColumnWidth(14, 250);
            ui->tableView->setColumnWidth(15, 80);

        }
        else {
            setWindowTitle("История переписи Принтеров");
            ui->label->hide();
            ui->pushButton->hide();
            ui->lineEdit->hide();
            per.prepare("SELECT id, Data_mod, USER_mod,action,id_zap,OTDELID, kab, model, name, IP, "
                        "inv,ser,roz,comment,spisan,STATUS FROM printer_history;");
            per.exec();
            per.next();
            qmodel->setQuery(per);
            qmodel->setHeaderData(0, Qt::Horizontal, "ИД");
            qmodel->setHeaderData(1, Qt::Horizontal, "Дата модификации");
            qmodel->setHeaderData(2, Qt::Horizontal, "Изменил");
            qmodel->setHeaderData(3, Qt::Horizontal, "Действие");
            qmodel->setHeaderData(4, Qt::Horizontal, "ИД изм. записи");
            qmodel->setHeaderData(5, Qt::Horizontal, "ИД отдела");
            qmodel->setHeaderData(6, Qt::Horizontal, "Кабинет");
            qmodel->setHeaderData(7, Qt::Horizontal, "Модель");
            qmodel->setHeaderData(8, Qt::Horizontal, "Имя");
            qmodel->setHeaderData(9, Qt::Horizontal, "IP");
            qmodel->setHeaderData(10, Qt::Horizontal, "Инвентарный");
            qmodel->setHeaderData(11, Qt::Horizontal, "Серийный");
            qmodel->setHeaderData(12, Qt::Horizontal, "Розетка");
            qmodel->setHeaderData(13, Qt::Horizontal, "Комментарий");
            qmodel->setHeaderData(14, Qt::Horizontal, "Статус списания");
            qmodel->setHeaderData(15, Qt::Horizontal, "Статус");
        ui->tableView->setModel(qmodel);
        ui->tableView->setColumnWidth(0, 50);
        ui->tableView->setColumnWidth(1, 200);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 500);
        ui->tableView->setColumnWidth(4, 80);
        ui->tableView->setColumnWidth(5, 80);
        ui->tableView->setColumnWidth(6, 80);
        ui->tableView->setColumnWidth(7, 250);
        ui->tableView->setColumnWidth(8, 180);
        ui->tableView->setColumnWidth(9, 80);
        ui->tableView->setColumnWidth(10, 80);
        ui->tableView->setColumnWidth(11, 80);
        ui->tableView->setColumnWidth(12, 80);
        ui->tableView->setColumnWidth(13, 80);
        ui->tableView->setColumnWidth(14, 250);
        ui->tableView->setColumnWidth(15, 80);

}


    }
}

Search::~Search()
{
    delete ui;
}

void Search::on_pushButton_clicked()
{
    if(id1==0){
        pc.prepare("select c.id, c.maker, c.model, c.name, c.inv, c.ser, c.CPU, c.cpuZ, c.RAM, c.ramZ, c.HDD, c.hddZ, c.OSid, c.eToken, c.IP, c.spisan, c.blokhost, c.comment from Comp c where not exists (select p.COMPID from perepis p where p.COMPID=c.id) and c.inv like :inv;");
        pc.bindValue(":inv","%"+ui->lineEdit->text()+"%");
        pc.exec();
        pc.next();
         qmodel->setQuery(pc);
         ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    }
    else if (id1==1) {
        mon.prepare("select m.id,m.inv,m.maker,m.model,m.ser,m.spisan,m.comment from Monitor m where not exists (select p.MONITORID from perepis p where p.MONITORID=m.id) and m.inv like :inv;");
        mon.bindValue(":inv","%"+ui->lineEdit->text()+"%");
        mon.exec();
        mon.next();
        qmodel->setQuery(mon);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    }

}

void Search::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(id1==2 || id1==3){

    }
    else{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   vib = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   //qDebug()<<vib;
   emit Vibor(vib);
   MdiAreaSearch->closeActiveSubWindow();}
}

void Search::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}

