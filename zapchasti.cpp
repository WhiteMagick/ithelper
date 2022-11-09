#include "zapchasti.h"
#include "ui_zapchasti.h"
#include "model.h"
#include "defhistory.h"
#include "delegatefromzap.h"
#include <addzapprn.h>
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlError>
#include <QInputDialog>
#include <QClipboard>
#include <QMenu>
#include <QDate>

Zapchasti::Zapchasti(int idP, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Zapchasti)
{
    ui->setupUi(this);
    MdiAreaZap = md;
    idPrn = idP;

    if(idPrn==0){
    setWindowTitle("Запчасти для принтеров");
    qryModel=new QSqlQueryModel(this);
    qry.prepare("Select id, name, count, listPrn, comment, status_zap from ZapchastiPrn where status_zap = 0;");
    qry.exec();
    qryModel->setQuery(qry);
    qryModel->setHeaderData(0, Qt::Horizontal, "ID");
    qryModel->setHeaderData(1, Qt::Horizontal, "Название");
    qryModel->setHeaderData(2, Qt::Horizontal, "Кол-во");
    qryModel->setHeaderData(3, Qt::Horizontal, "для принтера ид");
    qryModel->setHeaderData(4, Qt::Horizontal, "Коментарий");
    qryModel->setHeaderData(5, Qt::Horizontal, "Статус");
    ui->tableView->setItemDelegate(new delegatefromZap(ui->tableView));
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    proxi = new QSortFilterProxyModel(this);
    proxi->setSourceModel(qryModel);
    ui->tableView->setModel(proxi);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalScrollBar();
    ui->tableView->setColumnWidth(0, 30);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(2, 50);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 250);
    ui->tableView->setColumnWidth(5, 50);

    click=0;
    }else {
        setWindowTitle("Выбор запчасти");

        QSqlQuery getSlid;
        getSlid.prepare("SELECT model from Printer where id = :id;");
        getSlid.bindValue(":id", idPrn);
        getSlid.exec();
        getSlid.next();
        ui->pushButton_Add->hide();
        ui->pushButton_del->hide();
        ui->pushButton_hide->hide();
        ui->pushButton_hist->hide();
        ui->pushButton_refresh->hide();
        qryModel=new QSqlQueryModel(this);

        qry.prepare("Select id, name, count, listPrn, comment, status_zap from ZapchastiPrn where status_zap = 0 and listPrn like :idPr;");
        qry.bindValue(":idPr","%"+getSlid.value(0).toString()+"%");
        qry.exec();

        qryModel->setQuery(qry);
        qryModel->setHeaderData(1, Qt::Horizontal, "Название");
        qryModel->setHeaderData(2, Qt::Horizontal, "Кол-во");
        qryModel->setHeaderData(3, Qt::Horizontal, "для принтера ид");
        qryModel->setHeaderData(4, Qt::Horizontal, "Коментарий");
        qryModel->setHeaderData(5, Qt::Horizontal, "Статус");
        ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        proxi = new QSortFilterProxyModel(this);
        proxi->setSourceModel(qryModel);
        ui->tableView->setModel(proxi);
        ui->tableView->setSortingEnabled(true);
        ui->tableView->horizontalScrollBar();
        ui->tableView->setColumnWidth(0, 30);
        ui->tableView->setColumnWidth(1, 300);
        ui->tableView->setColumnWidth(2, 30);
        ui->tableView->setColumnWidth(3, 100);
        ui->tableView->setColumnWidth(4, 250);
        ui->tableView->setColumnWidth(5, 30);

        click=0;

    }
}

Zapchasti::~Zapchasti()
{
    delete ui;
}

void Zapchasti::on_pushButton_Add_clicked()
{
    addZapPrn *prn = new addZapPrn(0,MdiAreaZap);
    QMdiSubWindow *subWindow = MdiAreaZap->addSubWindow(prn);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 580);
    subWindow->show();
    connect(prn,SIGNAL(Reselect()),this,SLOT(reselect()));


}

void Zapchasti::on_pushButton_del_clicked()
{
   QModelIndex currentDiscount  = ui->tableView->currentIndex();
   id = proxi->data(proxi->index(currentDiscount.row(),0)).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'ZapchastiPrn' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    QSqlQuery qryDelSubdiv;
    qryDelSubdiv.prepare("DELETE from ZapchastiPrn where id=(:id);");
    qryDelSubdiv.bindValue(":id",id);
    QMessageBox::StandardButton msg1;
    msg1=QMessageBox::question(this,"Внимание","Если у запчасти есть история то лучше поставить статус (Архив)."
                                               " Чтобы удалить нужно сначала очистить историю с выбранной записью.",QMessageBox::Yes|QMessageBox::No);
    if(msg1 == QMessageBox::Yes){
        if(qryDelSubdiv.exec()){
        qryDelSubdiv.next();
        qry.exec();
        qryModel->setQuery(qry);
        }else {
        QMessageBox::about(this,"Error",qryDelSubdiv.lastError().text());
        }


   }
    else if(msg1==QMessageBox::No){

    }
   }

}

void Zapchasti::on_pushButton_hide_clicked()
{
    if(click==0){
        click=1;

        ui->pushButton_hide->setIcon(QIcon(":/hide.png"));
        ui->pushButton_hide->setIconSize(QSize(25,25));
        qry.prepare("Select id, name, count, listPrn, comment, status_zap from ZapchastiPrn;");
        qry.exec();
        qryModel->setQuery(qry);
    }else if(click==1){
        click=0;
        ui->pushButton_hide->setIcon(QIcon(":/file.png"));
        ui->pushButton_hide->setIconSize(QSize(25,25));
        qry.prepare("Select id, name, count, listPrn, comment, status_zap from ZapchastiPrn where status_zap = 0;");
        qry.exec();
        qryModel->setQuery(qry);

    }
}

void Zapchasti::on_pushButton_clicked()
{
    proxi->setFilterWildcard("*"+ui->lineEdit->text()+"*");
    proxi->setFilterKeyColumn(1);
}

void Zapchasti::history()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();
    int id2 = proxi->data(proxi->index(currentDiscount.row(),0)).toInt();
    defHistory *hist = new defHistory(MdiAreaZap,id2,0);
    QMdiSubWindow *subWindow = MdiAreaZap->addSubWindow(hist);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 580);
    subWindow->show();
}

void Zapchasti::setStatus()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    int id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    int st = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),5),0).toInt();
    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'ZapchastiPrn' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
         QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {

        if(st == 0){
    QSqlQuery qryDel;
    qryDel.prepare("UPDATE ZapchastiPrn SET status_zap = :st where id=(:id);");
      qryDel.bindValue(":st",1);
    qryDel.bindValue(":id",id);
        if(qryDel.exec()){
            qry.exec();
            qry.next();
            qryModel->setQuery(qry);
        }else{
           QMessageBox::about(this,"Error",qryDel.lastError().text());
        }
        }else if(st==1){
            QSqlQuery qryDel;
            qryDel.prepare("UPDATE ZapchastiPrn SET status_zap = :st where id=(:id);");
              qryDel.bindValue(":st",0);
            qryDel.bindValue(":id",id);
                if(qryDel.exec()){
                    qry.exec();
                    qry.next();
                    qryModel->setQuery(qry);
                }else{
                   QMessageBox::about(this,"Error",qryDel.lastError().text());
                }
        }
    }
}

void Zapchasti::on_tableView_doubleClicked(const QModelIndex &index)
{

   QModelIndex currentDiscount  = ui->tableView->currentIndex();
   int id2 = proxi->data(proxi->index(currentDiscount.row(),0)).toInt();
   if(idPrn==0){
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'ZapchastiPrn' AND Link = :link;");
   log_prov.bindValue(":link",id2);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
   addZapPrn *prn = new addZapPrn(id2,MdiAreaZap);
   QMdiSubWindow *subWindow = MdiAreaZap->addSubWindow(prn);
   QScreen *screen = qApp->primaryScreen();
   int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
   int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
   subWindow->setGeometry(x, y, 670, 580);
   subWindow->show();
   connect(prn,SIGNAL(Reselect()),this,SLOT(reselect()));
   QSqlQuery log;
   log.prepare("exec addLog :tbl, :link;");
   log.bindValue(":tbl","ZapchastiPrn");
   log.bindValue(":link",id2);
   log.exec();
  }
    }else {  //выбор запчасти для установки
        QSqlQuery count;
       count.prepare("select count from ZapchastiPrn where id = :idZ;");
       count.bindValue(":idZ",id2);
       count.exec();
       count.next();
       if(count.value(0).toInt()==0){
           QMessageBox::warning(this,"Error","Количество запчастей равно 0");
       }else{
        QSqlQuery inst;
        inst.prepare("insert into InstallZap (idZap, idPrn, dateInst) VALUES (:idZap, :idPrn, :dateInst);");
        inst.bindValue(":idZap",id2);
        inst.bindValue(":idPrn",idPrn);
        inst.bindValue(":dateInst",QDate::currentDate().toString("dd.MM.yyyy"));

        if(!inst.exec()){
           QMessageBox::warning(this,"Error",inst.lastError().text());
        }
        MdiAreaZap->closeActiveSubWindow();
        MdiAreaZap->activatePreviousSubWindow();
        emit Complite();
   }
}

}

void Zapchasti::on_pushButton_refresh_clicked()
{
    qry.exec();
    proxi->setFilterWildcard("");

}

void Zapchasti::on_pushButton_hist_clicked()
{
    defHistory *hist = new defHistory(MdiAreaZap,0,0);
    QMdiSubWindow *subWindow = MdiAreaZap->addSubWindow(hist);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 580);
    subWindow->show();
}

void Zapchasti::customMenu(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *op = new QAction(tr("История записи"),this);
    connect(op,SIGNAL(triggered()),this,SLOT(history()));
    menu->addAction(op);
    QAction *st = new QAction(tr("Установить/снять статус архивной записи"),this);
    connect(st,SIGNAL(triggered()),this,SLOT(setStatus()));
    menu->addAction(st);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

}

void Zapchasti::reselect()
{
    qry.exec();
    qryModel->setQuery(qry);
    proxi->setSourceModel(qryModel);
    proxi->setFilterWildcard("");

}

void Zapchasti::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Enter:{
                        if(ui->lineEdit->hasFocus()){
                           on_pushButton_clicked();
                        }
                        break;
                        }
    case Qt::Key_Escape:{

                        break;
                        }
}
}
