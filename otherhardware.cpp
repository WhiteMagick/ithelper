#include "otherhardware.h"
#include "ui_otherhardware.h"
#include "addotherh.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlError>
#include <QInputDialog>
#include <QClipboard>
#include <QMenu>
#include <QKeyEvent>

OtherHardware::OtherHardware(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtherHardware)
{
    MdiAreaOther = md;
    ui->setupUi(this);
    setWindowTitle("Другое оборудование");
    qryModel=new QSqlQueryModel(this);
    qry.prepare("select id, kab, name, model, inv, koment, STATUS from OtherHardware;");
    qry.exec();
    qryModel->setQuery(qry);
    qryModel->setHeaderData(1, Qt::Horizontal, "Кабинет");
    qryModel->setHeaderData(2, Qt::Horizontal, "Производитель");
    qryModel->setHeaderData(3, Qt::Horizontal, "Модель");
    qryModel->setHeaderData(4, Qt::Horizontal, "Инвентарный номер");
    qryModel->setHeaderData(5, Qt::Horizontal, "Комментарий");
    qryModel->setHeaderData(6, Qt::Horizontal, "Статус списания");
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(qryModel);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalScrollBar();
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    ui->comboBox->clear();
    ui->comboBox->addItem("Инв № содержит",QVariant(0));
    ui->comboBox->addItem("Производитель содержит",QVariant(1));
    ui->comboBox->addItem("Статус списания",QVariant(2));
    ui->comboBox->addItem("Коментарий",QVariant(3));
    ui->comboBox->addItem("Модель содержит",QVariant(1));

}

OtherHardware::~OtherHardware()
{
    delete ui;
}

void OtherHardware::on_Button_add_clicked()
{
    addOtherH *other = new addOtherH(0,MdiAreaOther);
    QMdiSubWindow *subWindow = MdiAreaOther->addSubWindow(other);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 627, 300);
    subWindow->show();
     connect(other,SIGNAL(Reselect()),this,SLOT(reselect()));
}

void OtherHardware::on_Button_dell_clicked()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'OtherHardware' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    QSqlQuery qryDelSubdiv;
    qryDelSubdiv.prepare("DELETE from OtherHardware where id=(:id);");
    qryDelSubdiv.bindValue(":id",id);
    QMessageBox::StandardButton msg;
    msg=QMessageBox::question(this,"Внимание","Удаление записи возможно только в том случае если запись не связана с другими таблицами.",QMessageBox::Yes|QMessageBox::No);
    if(msg == QMessageBox::Yes){
        if(qryDelSubdiv.exec()){
            qry.exec();
            qry.next();
            qryModel->setQuery(qry);
        }else{
           QMessageBox::about(this,"Error",qryDelSubdiv.lastError().text());
        }
    }
   }
}

void OtherHardware::on_Button_copy_clicked()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();

   bool ok;
   int kol = QInputDialog::getInt(this,tr("Сколько копий"),tr("кол-во"),1,0,100,1,&ok);
   if(ok){
       for(kol;kol>0;kol--){
              copy.prepare("exec copyOtherH :id,:inv");
              copy.bindValue(":id",id);
              copy.bindValue(":inv","б/н"+QString::number(kol));
              copy.exec();
              qry.exec();
              qry.next();
              qryModel->setQuery(qry);
       }
   }
}

void OtherHardware::on_Button_update_clicked()
{
    qry.exec();
    qry.next();
    qryModel->setQuery(qry);
}

void OtherHardware::on_pushButton_search_clicked()
{
    switch (ui->comboBox->currentIndex()) {
    case 0:{

        search.prepare("select id, kab, name, model, inv, koment, STATUS from OtherHardware where inv like :inv;");
        search.bindValue(":inv","%"+ui->lineEdit->text()+"%");
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 1:{
        search.prepare("select id, kab, name, model, inv, koment, STATUS from OtherHardware where name = :name;");
        search.bindValue(":name","%"+ui->lineEdit->text()+"%");
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 2:{
        search.prepare("select id, kab, name, model, inv, koment, STATUS from OtherHardware where STATUS = :STATUS;");
        search.bindValue(":STATUS",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 3:{
        search.prepare("select id, kab, name, model, inv, koment, STATUS from OtherHardware where koment = :koment;");
        search.bindValue(":koment","%"+ui->lineEdit->text()+"%");
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 4:{
        search.prepare("select id, kab, name, model, inv, koment, STATUS from OtherHardware where model = :model;");
        search.bindValue(":model",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    }
}

void OtherHardware::on_comboBox_activated(int index)
{
    if(index==3){
        ui->lineEdit->setPlaceholderText("0-не списан, 1-списан, 2-на списании");
    }
    else {
        ui->lineEdit->setPlaceholderText("");
    }
}

void OtherHardware::reselect()
{
    qry.exec();
    qry.next();
    qryModel->setQuery(qry);
}

void OtherHardware::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'OtherHardware' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
       addOtherH *other = new addOtherH(id,MdiAreaOther);
       QMdiSubWindow *subWindow = MdiAreaOther->addSubWindow(other);
       QScreen *screen = qApp->primaryScreen();
       int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
       int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
       subWindow->setGeometry(x, y, 627, 300);
       subWindow->show();
        connect(other,SIGNAL(Reselect()),this,SLOT(reselect()));
    QSqlQuery log;
    log.prepare("exec addLog :tbl, :link;");
    log.bindValue(":tbl","OtherHardware");
    log.bindValue(":link",id);
    log.exec();
   }
}

void OtherHardware::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}
