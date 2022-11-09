#include "kolonki.h"
#include "ui_kolonki.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlError>
#include <QInputDialog>
#include <QClipboard>
#include <QMenu>
#include "addkolonki.h"
#include "search.h"
#include "delegatefromkolonky.h"
Kolonki::Kolonki(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kolonki)
{
    ui->setupUi(this);
    MdiAreaKolonki = md;
    setWindowTitle("Колонки по кабинетам");
    qryModel=new QSqlQueryModel(this);
    qry.prepare("Select k.id, k.Kab, k.How_use,k.Kolonki, k.Instal, k.Date_in, k.Date_out, k.Comment, k.GUID, k.Sost from Kolonky2 k where k.Sost = 0;");
    qry.exec();
    qryModel->setQuery(qry);
    qryModel->setHeaderData(1, Qt::Horizontal, "Кабинет");
    qryModel->setHeaderData(2, Qt::Horizontal, "Пользователь");
    qryModel->setHeaderData(3, Qt::Horizontal, "Колонки");
    qryModel->setHeaderData(4, Qt::Horizontal, "Кто установил");
    qryModel->setHeaderData(5, Qt::Horizontal, "Дата установки");
    qryModel->setHeaderData(6, Qt::Horizontal, "Дата возврата");
    qryModel->setHeaderData(7, Qt::Horizontal, "Коментарий");
    qryModel->setHeaderData(8, Qt::Horizontal, "GUID");
    ui->tableView->setItemDelegate(new delegatefromkolonky(ui->tableView));
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(qryModel);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalScrollBar();
    click=0;
}

Kolonki::~Kolonki()
{
    delete ui;
}

void Kolonki::on_pushButton_Add_clicked()
{
    AddKolonki *kolonky = new AddKolonki(0,MdiAreaKolonki);
    QMdiSubWindow *subWindow = MdiAreaKolonki->addSubWindow(kolonky);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 580, 310);
    subWindow->show();
     connect(kolonky,SIGNAL(Reselect()),this,SLOT(reselect()));
}

void Kolonki::on_pushButton_Dell_clicked()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QString s = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),8),0).toString();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'Kolonky2' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    QSqlQuery qryDelSubdiv;
    qryDelSubdiv.prepare("DELETE from Kolonky2 where id=(:id);");
    qryDelSubdiv.bindValue(":id",id);
    QMessageBox::StandardButton msg1;
    msg1=QMessageBox::question(this,"Внимание","Удалить запись?",QMessageBox::Yes|QMessageBox::No);
    if(msg1 == QMessageBox::Yes){
    QMessageBox::StandardButton msg;
    msg=QMessageBox::question(this,"Внимание","Удалить историю этой записи?.",QMessageBox::Yes|QMessageBox::No);
    if(msg == QMessageBox::Yes){
        if(qryDelSubdiv.exec()){
            qryDelSubdiv.next();
            QSqlQuery delhist;
            delhist.prepare("delete from Kolonki_history where GUID = :GUID;");
            delhist.bindValue(":GUID",s);
            delhist.exec();
            reselect();
        }else{
           QMessageBox::about(this,"Error",qryDelSubdiv.lastError().text());
        }
    }else if(msg == QMessageBox::No){
        if(qryDelSubdiv.exec()){
            qryDelSubdiv.next();
             reselect();
        }else{
           QMessageBox::about(this,"Error",qryDelSubdiv.lastError().text());
        }
    }
   }
    else if(msg1==QMessageBox::No){

    }
   }


}

void Kolonki::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'Kolonky2' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    AddKolonki *kolonky = new AddKolonki(id,MdiAreaKolonki);
    QMdiSubWindow *subWindow = MdiAreaKolonki->addSubWindow(kolonky);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 580, 310);
    subWindow->show();
     connect(kolonky,SIGNAL(Reselect()),this,SLOT(reselect()));
     QSqlQuery log;
     log.prepare("exec addLog :tbl, :link;");
     log.bindValue(":tbl","Kolonky2");
     log.bindValue(":link",id);
     log.exec();
     }
}

void Kolonki::reselect()
{
    qry.exec();
    qryModel->setQuery(qry);
}

void Kolonki::customMenu(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *op = new QAction(tr("История записи"),this);
    connect(op,SIGNAL(triggered()),this,SLOT(history()));
    menu->addAction(op);
     menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void Kolonki::history()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();
    QString s = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),8),0).toString();
    Search *poisk = new Search(MdiAreaKolonki,2,s);
    QMdiSubWindow *subWindow = MdiAreaKolonki->addSubWindow(poisk);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 350);
    subWindow->show();

}

void Kolonki::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}



void Kolonki::on_pushButton_hide_clicked()
{
    if(click==0){
        click=1;
        ui->pushButton_hide->setIcon(QIcon(":/hide.png"));
        ui->pushButton_hide->setIconSize(QSize(25,25));
        qry.prepare("Select k.id, k.Kab, k.How_use,k.Kolonki, k.Instal, k.Date_in, k.Date_out, k.Comment, k.GUID,k.Sost from Kolonky2 k;");
        qry.exec();
        qryModel->setQuery(qry);
    }else if(click==1){
        click=0;
        ui->pushButton_hide->setIcon(QIcon(":/file.png"));
        ui->pushButton_hide->setIconSize(QSize(25,25));
        qry.prepare("Select k.id, k.Kab, k.How_use,k.Kolonki, k.Instal, k.Date_in, k.Date_out, k.Comment, k.GUID,k.Sost from Kolonky2 k where k.Sost = 0;");
        qry.exec();
        qryModel->setQuery(qry);

    }
}
