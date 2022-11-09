#include "kartridj.h"
#include "ui_kartridj.h"
#include "addkartridj.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlError>
#include <QInputDialog>
#include <QClipboard>
#include <QMenu>
#include <QDate>
#include <QModelIndex>
#include "delegatekartr.h"
#include "login.h"



kartridj::kartridj(int idP,int idPrnP, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kartridj)
{
    ui->setupUi(this);
    MdiAreakar = md;
    Login *k = new Login;
    qDebug()<<idPrnP;
    idPrn = idP;

    prn = idPrnP;
    qDebug()<<prn;
    ui->tabWidget->setCurrentIndex(0);
    if(idPrn==0){
    setWindowTitle("Картриджи (справочник и склад)");
    qryModel=new QSqlQueryModel(this);
    qry.prepare("Select id, name, res from SalaryKartridj;");
    qry.exec();

    qryModel->setQuery(qry);
    //qryModel->setHeaderData(0, Qt::Horizontal, "ID");
    qryModel->setHeaderData(1, Qt::Horizontal, "Название");
    qryModel->setHeaderData(2, Qt::Horizontal, "Ресурс");

   // ui->tableView_salary->setItemDelegate(new delegatefromZap(ui->tableView_salary));
    ui->tableView_salary->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView_salary,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
    ui->tableView_salary->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_salary->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    proxi = new QSortFilterProxyModel(this);
    proxi->setSourceModel(qryModel);
    ui->tableView_salary->setModel(proxi);
    ui->tableView_salary->setSortingEnabled(true);
    ui->tableView_salary->horizontalScrollBar();
    ui->tableView_salary->hideColumn(0);
   // ui->tableView_salary->setColumnWidth(0, 30);
    ui->tableView_salary->setColumnWidth(1, 300);
    ui->tableView_salary->setColumnWidth(2, 50);


    //----------------------------------------------------------
    delegatekartr* delegate = new delegatekartr(ui->tableView_sklad);
       ui->tableView_sklad->setItemDelegateForColumn(1,delegate);
    model2 = new QSqlTableModel(this,k->db);
    model2->setTable("SkladKartridj");
    model2->setEditStrategy(QSqlTableModel::OnRowChange);
    model2->select();
    model2->setHeaderData(1, Qt::Horizontal, tr("Name"));
   // ui->tableView_sklad->setEditTriggers(QAbstractItemView::EditKeyPressed);

    //proxi2 = new QSortFilterProxyModel(this);
    //proxi2->setSourceModel(model2);
    ui->tableView_sklad->setModel(model2);
    ui->tableView_sklad->setSortingEnabled(true);
    ui->tableView_sklad->hideColumn(0);
    ui->tableView_sklad->setColumnWidth(2, 60);
    ui->tableView_sklad->setColumnWidth(1,300);
    ui->tableView_sklad->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);

    click=0;


    }else{
        setWindowTitle("Выбор картриджа для установки");
        ui->tab->deleteLater();
        ui->tableView_salary->deleteLater();
        ui->tab_2->deleteLater();
        ui->tableView_sklad->deleteLater();
        ui->tabWidget->deleteLater();

        //ui->tabWidget->setCurrentIndex(1);
       // ui->tab->setDisabled(true);
        ui->pushButton_add->hide();
        ui->pushButton_del->hide();
        ui->pushButton_upd->hide();
  vibor = new QTableView;
  //ui->gridLayout->deleteLater();
  ui->gridLayout_2->deleteLater();
   ui->gridLayout_3->deleteLater();
    ui->gridLayout_4->deleteLater();
    ui->horizontalSpacer->changeSize(0,0,QSizePolicy::Ignored);
//    QGridLayout *grid = new QGridLayout(this);
//    setLayout(grid);
     ui->gridLayout->addWidget(vibor);

    qryModel=new QSqlQueryModel(this);
    vibor->setModel(qryModel);
    qry.prepare("select s.id, k.id, k.name, s.kol from SkladKartridj s "
                "inner join SalaryKartridj k on k.id = s.idKart "
                "left join SovmestimostKartPrn so on so.idKart = k.id "
                "left join Printer p on p.id = so.idPrn "
                "where so.idPrn = :id;");
    qry.bindValue(":id",idPrn);
    qry.exec();

    qryModel->setQuery(qry);
    vibor->hideColumn(0);
    vibor->hideColumn(1);
    connect(vibor,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickforVibor(QModelIndex)));
   // connect(vibor,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_tableView_vibor(QModelIndex)));


    }

}

kartridj::~kartridj()
{
    delete ui;
}

void kartridj::setvale(int s)
{
    qDebug()<<s;
}

void kartridj::on_pushButton_add_clicked()
{
    if(ui->tabWidget->currentIndex()==0){
        AddKartridj *kar = new AddKartridj(0,MdiAreakar);
        QMdiSubWindow *subWindow = MdiAreakar->addSubWindow(kar);
        QScreen *screen = qApp->primaryScreen();
        int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
        int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
        subWindow->setGeometry(x, y, 410, 450);
        subWindow->show();
        connect(kar,SIGNAL(Reselect()),this,SLOT(reselect()));

     }else if(ui->tabWidget->currentIndex()==1){
        model2->insertRow(model2->rowCount());
    }
}

void kartridj::on_pushButton_del_clicked()
{
    if(ui->tabWidget->currentIndex()==0){
        QModelIndex currentDiscount  = ui->tableView_salary->currentIndex();                                       //получение значения первого столбца таблицы.
      int id = ui->tableView_salary->model()->data(ui->tableView_salary->model()->index(currentDiscount.row(),0),0).toInt();
        QSqlQuery log_prov;
        log_prov.prepare("select user_name from JURNAL where tbl = 'SalaryKartridj' AND Link = :link;");
        log_prov.bindValue(":link",id);
        log_prov.exec();
        log_prov.next();
        if(log_prov.value(0).toString().length()>0){
             QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
        }else {
            QSqlQuery qryDelSubdiv;
            qryDelSubdiv.prepare("DELETE from SalaryKartridj where id=(:id);");
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
    else if(ui->tabWidget->currentIndex()==1){
        int row = ui->tableView_sklad->currentIndex().row();
       if(row>=0){
           model2->removeRow(row);
       }else{
           qDebug("ska");
       }
    }

}

void kartridj::on_pushButton_upd_clicked()
{
    if(ui->tabWidget->currentIndex()==0){
        qry.exec();
        qryModel->setQuery(qry);

    }else if(ui->tabWidget->currentIndex()==1){

        model2->select();
    }
}

void kartridj::reselect()
{
    qry.exec();
    qryModel->setQuery(qry);
    proxi->setSourceModel(qryModel);
    proxi->setFilterWildcard("");
}

void kartridj::on_tableView_salary_doubleClicked(const QModelIndex &index)
{
    QModelIndex currentDiscount  = ui->tableView_salary->currentIndex();                                       //получение значения первого столбца таблицы.
  int id = ui->tableView_salary->model()->data(ui->tableView_salary->model()->index(currentDiscount.row(),0),0).toInt();

    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'SalaryKartridj' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
         QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {

    AddKartridj *kar = new AddKartridj(id,MdiAreakar);
    QMdiSubWindow *subWindow = MdiAreakar->addSubWindow(kar);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 410, 450);
    subWindow->show();
    connect(kar,SIGNAL(Reselect()),this,SLOT(reselect()));
    QSqlQuery log;
    log.prepare("exec addLog :tbl, :link;");
    log.bindValue(":tbl","SalaryKartridj");
    log.bindValue(":link",id);
    log.exec();
    }
}

void kartridj::on_tableView_sklad_doubleClicked(const QModelIndex &index)
{

}

void kartridj::doubleClickforVibor(const QModelIndex &index)
{
    QModelIndex currentDiscount  = vibor->currentIndex();                                       //получение значения первого столбца таблицы.
   int id2 = vibor->model()->data(vibor->model()->index(currentDiscount.row(),0),0).toInt();
   int idK = vibor->model()->data(vibor->model()->index(currentDiscount.row(),1),0).toInt();
    QSqlQuery count;
   count.prepare("select kol from SkladKartridj where id = :idZ;");
   count.bindValue(":idZ",id2);
   count.exec();
   count.next();
   if(count.value(0).toInt()==0){
       QMessageBox::warning(this,"Error","Количество  кртриджей равно 0");
   }else{

       bool bOk;
       QString str = QInputDialog::getText( 0,
                                            "Установка картриджа",
                                            "Введите дату в формате dd.MM.yyyy:",
                                            QLineEdit::Normal,
                                            QDate::currentDate().toString("dd.MM.yyyy"),
                                            &bOk
                                           );
       if (bOk && !str.isEmpty()) {

           bool ok;
               int i = QInputDialog::getInt(this, tr("Установка картриджа"),
                                            tr("Кол-во отпечатков:"), 0, 0, 1000000, 1, &ok);
               if (ok){
                    qDebug()<<"id kart="+QString::number(idK)+"  id prnSL="+QString::number(idPrn)+"id prn perepis = "+QString::number(prn);
                       QSqlQuery inst;
                       inst.prepare("insert into Kartridj_history (date, idPrnPereris, idPrnSl, idKart, kolStr) VALUES (:date, :idPrnPereris, :idPrnSl, :idKart, :kolStr);");
                       inst.bindValue(":idKart",idK);
                       inst.bindValue(":idPrnSl",idPrn);
                       inst.bindValue(":date",str);
                       inst.bindValue(":kolStr",i);
                       inst.bindValue(":idPrnPereris",prn);

                       if(!inst.exec()){
                          QMessageBox::warning(this,"Error",inst.lastError().text());
                       }
                       MdiAreakar->closeActiveSubWindow();
                       MdiAreakar->activatePreviousSubWindow();
                       emit Complite();


               }else{
                   QMessageBox::warning(this,"Error","Отмена операции");
                   MdiAreakar->closeActiveSubWindow();
                   MdiAreakar->activatePreviousSubWindow();
               }
       }else{
           QMessageBox::warning(this,"Error","Отмена операции");
               MdiAreakar->closeActiveSubWindow();
               MdiAreakar->activatePreviousSubWindow();
       }





}

}

void kartridj::closeEvent(QCloseEvent *event)
{

}

void kartridj::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

      }
}
