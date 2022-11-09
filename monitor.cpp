#include "monitor.h"
#include "ui_monitor.h"
#include "addmon.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlError>
#include <QInputDialog>
#include <QClipboard>
#include <QMenu>
#include <QKeyEvent>

Monitor::Monitor(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Monitor)
{
    MdiAreaMon = md;
    ui->setupUi(this);
    setWindowTitle("Список мониторов");
    qryModel=new QSqlQueryModel(this);
    qry.prepare("select id, maker, model, inv, ser, spisan, comment, year from Monitor;");
    qry.exec();
    qryModel->setQuery(qry);
    qryModel->setHeaderData(1, Qt::Horizontal, "Производитель");
    qryModel->setHeaderData(2, Qt::Horizontal, "Модель");
    qryModel->setHeaderData(3, Qt::Horizontal, "Инвентарный номер");
    qryModel->setHeaderData(4, Qt::Horizontal, "Серийный номер");
    qryModel->setHeaderData(5, Qt::Horizontal, "Списан");
    qryModel->setHeaderData(6, Qt::Horizontal, "Комментарий");
    qryModel->setHeaderData(7, Qt::Horizontal, "Год");
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(qryModel);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalScrollBar();
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    ui->comboBox->clear();
    ui->comboBox->addItem("Инв № содержит",QVariant(0));
    ui->comboBox->addItem("Модель содержит",QVariant(1));
    ui->comboBox->addItem("Статус списания",QVariant(2));
    ui->comboBox->addItem("Год выпуска",QVariant(3));

    //qry.next();

}

Monitor::~Monitor()
{
    delete ui;
}

void Monitor::on_pushButton_clicked() //добавление
{
    addMon *mon = new addMon(0,MdiAreaMon);
    QMdiSubWindow *subWindow = MdiAreaMon->addSubWindow(mon);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 520, 310);
    subWindow->show();
     connect(mon,SIGNAL(Reselect()),this,SLOT(reselect()));

}

void Monitor::on_pushButton_2_clicked()  // удаление
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'Monitor' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    QSqlQuery qryDelSubdiv;
    qryDelSubdiv.prepare("DELETE from Monitor where id=(:id);");
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

void Monitor::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'Monitor' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    addMon *mon = new addMon(id,MdiAreaMon);
    QMdiSubWindow *subWindow = MdiAreaMon->addSubWindow(mon);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 520, 310);
    subWindow->show();
    connect(mon,SIGNAL(Reselect()),this,SLOT(reselect()));
    QSqlQuery log;
    log.prepare("exec addLog :tbl, :link;");
    log.bindValue(":tbl","Monitor");
    log.bindValue(":link",id);
    log.exec();
    }
}

void Monitor::reselect()
{
    search.exec();
    search.next();
    qryModel->setQuery(search);
}

void Monitor::on_pushButton_3_clicked()  //копирование
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();

   bool ok;
   int kol = QInputDialog::getInt(this,tr("Сколько копий"),tr("кол-во"),1,0,100,1,&ok);
   if(ok){
       for(kol;kol>0;kol--){
              copy.prepare("exec copyMon :id,:inv,:ser;");
              copy.bindValue(":id",id);
              copy.bindValue(":inv","б/н"+QString::number(kol));
              copy.bindValue(":ser","б/н"+QString::number(kol));
              copy.exec();
              qry.exec();
              qry.next();
              qryModel->setQuery(qry);
       }
   }
}

void Monitor::on_pushButton_4_clicked()  //поиск
{
    switch (ui->comboBox->currentIndex()) {
    case 0:{

        search.prepare("select id, maker, model, inv, ser, spisan, comment, year from Monitor where inv like :inv;");
        search.bindValue(":inv","%"+ui->lineEdit->text()+"%");
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 1:{
        search.prepare("select id, maker, model, inv, ser, spisan, comment, year from Monitor where model like :model;");
        search.bindValue(":model","%"+ui->lineEdit->text()+"%");
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 2:{
        search.prepare("select id, maker, model, inv, ser, spisan, comment, year from Monitor where spisan = :spisan;");
        search.bindValue(":spisan",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 3:{
        search.prepare("select id, maker, model, inv, ser, spisan, comment, year from Monitor where year = :year;");
        search.bindValue(":year",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    }
}

void Monitor::copyfromBuff()
{
    QAbstractItemModel *amodel = ui->tableView->model();
    QItemSelectionModel *model = ui->tableView->selectionModel();
    QModelIndexList list = model->selectedIndexes();

    qSort(list);

    if(list.size()<1)
        return;

    QString copy_table;
    QModelIndex last = list.last();
    QModelIndex previous = list.first();

    list.removeFirst();

    for(int i =0; i< list.size(); i++){
        QVariant data = amodel->data(previous);
        QString text = data.toString();

        QModelIndex index = list.at(i);
        copy_table.append(text);

        if(index.row() != previous.row()){
            copy_table.append('\n');
        }
        else {
            copy_table.append('\t');
        }
        previous=index;

    }
        copy_table.append(amodel->data(list.last()).toString());
        copy_table.append('\n');

       QClipboard *clip = QApplication::clipboard();
       clip->setText(copy_table);

}

void Monitor::customMenu(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *op = new QAction(tr("Копировать"),this);
    connect(op,SIGNAL(triggered()),this,SLOT(copyfromBuff()));
    menu->addAction(op);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void Monitor::on_comboBox_activated(int index)
{
    if(index==2){
        ui->lineEdit->setPlaceholderText("0-не списан, 1-списан, 2-на списании");
    }
    else {
        ui->lineEdit->setPlaceholderText("");
            }
}

void Monitor::on_pushButton_5_clicked()
{
    qry.exec();
    qry.next();
    qryModel->setQuery(qry);
}

void Monitor::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Enter:{
                        if(ui->lineEdit->hasFocus()){
                           on_pushButton_4_clicked();
                        }
                        break;
                        }
    case Qt::Key_Escape:{

                        break;
                        }
}
}
