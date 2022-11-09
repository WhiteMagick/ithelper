#include "printer.h"
#include "ui_printer.h"
#include "addprn.h"
#include "viborotdela.h"
#include "delegateforprinter.h"
#include "zapchasti.h"
#include "defhistory.h"
#include "search.h"
#include "kartridj.h"
#include <QProcess>
#include <QDebug>
#include <QDesktopServices>
#include <QChar>
#include <windows.h>
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QSqlError>
#include <QAxObject>
#include <QKeyEvent>

Printer::Printer(QSqlDatabase db, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Printer)
{
    mdiAreaPrinter = md;

    ui->setupUi(this);
    qryModel=new QSqlQueryModel(this);
    setWindowTitle("Перепись принтеров");
    ui->treeWidget->clear();
    //создание списка отделов и вывод в дерево
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
    QString sSql = QString("select name from Otdel order by CODE ASC;");
        QSqlQuery parentsQuery;
        if(!parentsQuery.exec(sSql))
            return;
        while(parentsQuery.next())
            {

                QString parentName = parentsQuery.value(0).toString();

                QTreeWidgetItem* parentItem = new QTreeWidgetItem();
                parentItem->setText(0,parentName);
                ui->treeWidget->setColumnCount(1);
                ui->treeWidget->headerItem()->setHidden(1);
                ui->treeWidget->addTopLevelItem(parentItem);
                parentItem->setIcon(0,QIcon(":/sub.png"));

        }
         ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

         ui->tableView->setModel(qryModel);

         ui->tableView->horizontalScrollBar();

         ui->comboBox->clear();
         ui->comboBox->addItem("Инв № содержит",QVariant(0));
         ui->comboBox->addItem("Модель содержит",QVariant(1));
         ui->comboBox->addItem("статус списания",QVariant (2));
         ui->comboBox->addItem("Кабинет",QVariant (3));
         ui->comboBox->addItem("Не актуальные данные",QVariant (4));
         ui->comboBox->addItem("Сломан",QVariant(5));


}

Printer::~Printer()
{
    delete ui;
}

void Printer::on_pushButtonAdd_clicked()
{
    AddPrn *prn = new AddPrn(0,ot,mdiAreaPrinter);
    QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(prn);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 470, 380);
    subWindow->show();
    connect(prn,SIGNAL(Reselect()),this,SLOT(reselect()));
}

void Printer::on_pushButtonDell_clicked()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    QString id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toString();
    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'Printer' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
         QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {
    QSqlQuery qryDel;
    qryDel.prepare("DELETE from Printer where id=(:id);");
    qryDel.bindValue(":id",id);
    QMessageBox::StandardButton msg;
    msg=QMessageBox::question(this,"Внимание","Удаление записи возможно только в том случае если запись не связана с другими таблицами.",QMessageBox::Yes|QMessageBox::No);
    if(msg == QMessageBox::Yes){
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

void Printer::on_tableView_doubleClicked(const QModelIndex &index)
{
    //переменная статус для того чтобы взять нужный номер отдела если редактируешь через поиск
if(status!=1){
QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
QSqlQuery log_prov;
log_prov.prepare("select user_name from JURNAL where tbl = 'Printer' AND Link = :link;");
log_prov.bindValue(":link",id);
log_prov.exec();
log_prov.next();
if(log_prov.value(0).toString().length()>0){
    QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
}else {
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
  // qDebug()<<id;
   AddPrn *prn = new AddPrn(id,ot,mdiAreaPrinter);
   QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(prn);
   QScreen *screen = qApp->primaryScreen();
   int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
   int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
   subWindow->setGeometry(x, y, 470, 380);
   subWindow->show();
   connect(prn,SIGNAL(Reselect()),this,SLOT(reselect()));
   QSqlQuery log;
   log.prepare("exec addLog :tbl, :link;");
   log.bindValue(":tbl","Printer");
   log.bindValue(":link",id);
   log.exec();
   }
}
else{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   ot = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),1),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'Printer' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
       AddPrn *prn = new AddPrn(id,ot,mdiAreaPrinter);
       QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(prn);
       QScreen *screen = qApp->primaryScreen();
       int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
       int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
       subWindow->setGeometry(x, y, 470, 380);
       subWindow->show();
       connect(prn,SIGNAL(Reselect()),this,SLOT(reselect()));
       QSqlQuery log;
       log.prepare("exec addLog :tbl, :link;");
       log.bindValue(":tbl","Printer");
       log.bindValue(":link",id);
       log.exec();
   }
}
}


void Printer::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    st=item->text(0);
    QModelIndex currentDiscount  = ui->treeWidget->currentIndex();
    QString n = ui->treeWidget->model()->data(ui->treeWidget->model()->index(currentDiscount.row(),0)).toString();
    qry.prepare("select p.id, p.otdelid,sl.id, p.kab as 'Кабинет', CONCAT(m.maker,' ',sl.model) as 'Модель', p.name as 'Имя', p.IP, p.inv as 'Инвентарный номер', p.ser as 'Серийный номер', p.roz as 'Розетка', p.comment as 'Коммернтарий',p.spisan, p.STATUS from Printer p "
                "left join Otdel ot on ot.CODE = p.otdelid "
                "left join SalaryPrn sl on sl.id = p.model "
                "left join SalaryMakerPRN m on sl.maker=m.id "
                "where ot.name like :nam;");
    qry.bindValue(":nam",st);
    qry.exec();
    qry.next();
    qryModel->setQuery(qry);
      ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
     ui->tableView->setItemDelegate(new DelegateForPrinter(ui->tableView));
    ui->tableView->horizontalScrollBar();
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->setColumnWidth(3, 60);
    ui->tableView->setColumnWidth(4, 150);
    ui->tableView->setColumnWidth(7, 130);
    ui->tableView->setColumnWidth(8, 120);


    otId.prepare("select CODE from Otdel where name = :name;");
            otId.bindValue(":name",st);
            otId.exec();
            otId.next();
            ot=otId.value(0).toInt();



}

void Printer::reselect()
{
    qry.exec();
    qry.next();
    qryModel->setQuery(qry);
}

void Printer::on_pushButtonSearch_clicked()
{
    switch (ui->comboBox->currentIndex()) {
    case 0:{
       searchPrn.prepare("select p.id, p.otdelid,sl.id, p.kab as 'Кабинет', CONCAT(m.maker,' ',sl.model) as 'Модель', p.name as 'Имя', p.IP, p.inv as 'Инвентарный номер', p.ser as 'Серийный номер', p.roz as 'Розетка', p.comment as 'Коммернтарий', p.spisan, p.STATUS from Printer p "
                         "left join Otdel ot on ot.CODE = p.otdelid "
                          "left join SalaryPrn sl on sl.id = p.model "
                         "left join SalaryMakerPRN m on sl.maker=m.id "
                        "where p.inv like :inv");
       searchPrn.bindValue(":inv","%"+ui->lineEdit->text()+"%");
       searchPrn.exec();
       searchPrn.next();
       qryModel->setQuery(searchPrn);
       ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
      ui->tableView->setItemDelegate(new DelegateForPrinter(ui->tableView));
     ui->tableView->horizontalScrollBar();
     ui->tableView->hideColumn(0);
     ui->tableView->hideColumn(1);
       ui->tableView->hideColumn(2);
        status = 1;
        break;
    }
    case 1:{
        searchPrn.prepare("select p.id, p.otdelid,sl.id, p.kab as 'Кабинет', CONCAT(m.maker,' ',sl.model) as 'Модель', p.name as 'Имя', p.IP, p.inv as 'Инвентарный номер', p.ser as 'Серийный номер', p.roz as 'Розетка', p.comment as 'Коммернтарий', p.spisan, p.STATUS from Printer p "
                          "left join Otdel ot on ot.CODE = p.otdelid "
                           "left join SalaryPrn sl on sl.id = p.model "
                          "left join SalaryMakerPRN m on sl.maker=m.id "
                         "where sl.model like :inv");
        searchPrn.bindValue(":inv","%"+ui->lineEdit->text()+"%");
        searchPrn.exec();
        searchPrn.next();
        qryModel->setQuery(searchPrn);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
       ui->tableView->setItemDelegate(new DelegateForPrinter(ui->tableView));
      ui->tableView->horizontalScrollBar();
      ui->tableView->hideColumn(0);
      ui->tableView->hideColumn(1);
        ui->tableView->hideColumn(2);
        status = 1;
        break;
    }
    case 2:{
        searchPrn.prepare("select p.id, p.otdelid,sl.id, p.kab as 'Кабинет', CONCAT(m.maker,' ',sl.model) as 'Модель', p.name as 'Имя', p.IP, p.inv as 'Инвентарный номер', p.ser as 'Серийный номер', p.roz as 'Розетка', p.comment as 'Коммернтарий', p.spisan, p.STATUS from Printer p "
                          "left join Otdel ot on ot.CODE = p.otdelid "
                           "left join SalaryPrn sl on sl.id = p.model "
                          "left join SalaryMakerPRN m on sl.maker=m.id "
                         "where p.spisan like :spisan");
        searchPrn.bindValue(":spisan",ui->lineEdit->text());
        searchPrn.exec();
        searchPrn.next();
        qryModel->setQuery(searchPrn);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
       ui->tableView->setItemDelegate(new DelegateForPrinter(ui->tableView));
      ui->tableView->horizontalScrollBar();
      ui->tableView->hideColumn(0);
      ui->tableView->hideColumn(1);
      ui->tableView->hideColumn(2);
        status = 1;
        break;
    }
    case 3:{
        searchPrn.prepare("select p.id, p.otdelid,sl.id, p.kab as 'Кабинет', CONCAT(m.maker,' ',sl.model) as 'Модель', p.name as 'Имя', p.IP, p.inv as 'Инвентарный номер', p.ser as 'Серийный номер', p.roz as 'Розетка', p.comment as 'Коммернтарий', p.spisan, p.STATUS from Printer p "
                          "left join Otdel ot on ot.CODE = p.otdelid "
                           "left join SalaryPrn sl on sl.id = p.model "
                          "left join SalaryMakerPRN m on sl.maker=m.id "
                         "where p.kab like :kab");
        searchPrn.bindValue(":kab",ui->lineEdit->text());
        searchPrn.exec();
        searchPrn.next();
        qryModel->setQuery(searchPrn);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
       ui->tableView->setItemDelegate(new DelegateForPrinter(ui->tableView));
      ui->tableView->horizontalScrollBar();
      ui->tableView->hideColumn(0);
      ui->tableView->hideColumn(1);
        status = 1;
        break;
    }
    case 4: {
        searchPrn.prepare("select p.id, p.otdelid,sl.id, p.kab as 'Кабинет', CONCAT(m.maker,' ',sl.model) as 'Модель', p.name as 'Имя', p.IP, p.inv as 'Инвентарный номер', p.ser as 'Серийный номер', p.roz as 'Розетка', p.comment as 'Коммернтарий', p.spisan, p.STATUS from Printer p "
                          "left join Otdel ot on ot.CODE = p.otdelid "
                           "left join SalaryPrn sl on sl.id = p.model "
                          "left join SalaryMakerPRN m on sl.maker=m.id "
                         "where p.STATUS='2'");
       // searchPrn.bindValue(":kab",ui->lineEdit->text());
        searchPrn.exec();
        searchPrn.next();
        qryModel->setQuery(searchPrn);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
       ui->tableView->setItemDelegate(new DelegateForPrinter(ui->tableView));
      ui->tableView->horizontalScrollBar();
      ui->tableView->hideColumn(0);
      ui->tableView->hideColumn(1);
        status = 1;

        break;
    }
    case 5: {
        searchPrn.prepare("select p.id, p.otdelid,sl.id, p.kab as 'Кабинет', CONCAT(m.maker,' ',sl.model) as 'Модель', p.name as 'Имя', p.IP, p.inv as 'Инвентарный номер', p.ser as 'Серийный номер', p.roz as 'Розетка', p.comment as 'Коммернтарий', p.spisan, p.STATUS from Printer p "
                          "left join Otdel ot on ot.CODE = p.otdelid "
                           "left join SalaryPrn sl on sl.id = p.model "
                          "left join SalaryMakerPRN m on sl.maker=m.id "
                         "where p.STATUS='1'");
       // searchPrn.bindValue(":kab",ui->lineEdit->text());
        searchPrn.exec();
        searchPrn.next();
        qryModel->setQuery(searchPrn);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
       ui->tableView->setItemDelegate(new DelegateForPrinter(ui->tableView));
      ui->tableView->horizontalScrollBar();
      ui->tableView->hideColumn(0);
      ui->tableView->hideColumn(1);
        status = 1;

        break;
    }
}
}

void Printer::on_comboBox_activated(int index)
{
    if(index==2){
        ui->lineEdit->setPlaceholderText("0-не списан, 1-списан, 2-на списании");
    }
   else if(index == 4||index==5)
    {
         ui->lineEdit->setEnabled(false);
    }
    else {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit->setPlaceholderText("");
            }
}

void Printer::on_pushButton_clicked()  //перемещение междцу отделами
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'Printer' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    ViborOtdela *vib = new ViborOtdela(mdiAreaPrinter,id,1);
    QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(vib);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 370, 460);
    subWindow->show();
    connect(vib,SIGNAL(Reselect()),this,SLOT(reselect()));
   }
}

void Printer::customMenu(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *teh = new QAction(tr("Установить/снять статус сломан"),this);
    connect(teh,SIGNAL(triggered()),this,SLOT(editSt()));
    menu->addAction(teh);

    QAction *st = new QAction(tr("Установить/снять статус Не актуальные данные"),this);
    connect(st,SIGNAL(triggered()),this,SLOT(editStActual()));
    menu->addAction(st);

    QAction *inst = new QAction(tr("Установить запчасть"),this);
    connect(inst,SIGNAL(triggered()),this,SLOT(instZap()));
    menu->addAction(inst);

    QAction *histZap = new QAction(tr("История установленных запчастей"),this);
    connect(histZap,SIGNAL(triggered()),this,SLOT(historyZap()));
    menu->addAction(histZap);


    QAction *instK = new QAction(tr("Установить картридж"),this);
    connect(instK,SIGNAL(triggered()),this,SLOT(instKart()));
    menu->addAction(instK);

    QAction *histKart = new QAction(tr("История установленных картриджей"),this);
    connect(histKart,SIGNAL(triggered()),this,SLOT(historyKart()));
    menu->addAction(histKart);

    QAction *his = new QAction(tr("История записи"),this);
    connect(his,SIGNAL(triggered()),this,SLOT(history()));
    menu->addAction(his);

    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

}

void Printer::editSt()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    int id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    int st = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),12),0).toInt();
    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'Printer' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
         QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {

        if(st == 0){
    QSqlQuery qryDel;
    qryDel.prepare("UPDATE Printer SET STATUS = :st where id=(:id);");
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
            qryDel.prepare("UPDATE Printer SET STATUS = :st where id=(:id);");
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

void Printer::editStActual()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    int id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    int st = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),12),0).toInt();
    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'Printer' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
         QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {

        if(st == 0){
    QSqlQuery qryDel;
    qryDel.prepare("UPDATE Printer SET STATUS = :st where id=(:id);");
      qryDel.bindValue(":st",2);
    qryDel.bindValue(":id",id);
        if(qryDel.exec()){
            qry.exec();
            qry.next();
            qryModel->setQuery(qry);
        }else{
           QMessageBox::about(this,"Error",qryDel.lastError().text());
        }
        }else if(st==2){
            QSqlQuery qryDel;
            qryDel.prepare("UPDATE Printer SET STATUS = :st where id=(:id);");
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

void Printer::on_Report_Button_clicked()
{
    QString tmp_s=  QApplication::applicationDirPath();
        // получаем указатель на Excel
            QAxObject *mExcel = new QAxObject("Excel.Application",this);
            //видимость
            mExcel->dynamicCall( "SetVisible(bool)", true );
            // на книги
            QAxObject *workbooks = mExcel->querySubObject("Workbooks");
            // на директорию, откуда грузить книг
            QAxObject *workbook = workbooks->querySubObject( "Open(const QString&)",tmp_s+ "/perepisPrn.xltx" );
            // на листы
            QAxObject *mSheets = workbook->querySubObject( "Sheets" );
            // указываем, какой лист выбрать
            QAxObject *StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant("list") );
            int i,j;
            i=3;
            QSqlQuery parentsQuery;

            reportQry="SELECT  c.name, p.kab, CONCAT(m.maker,' ',sl.model), p.name, p.IP, p.inv, p.ser, p.roz FROM Printer p "
                      "inner join Otdel c on c.CODE = p.otdelid "
                     "left join SalaryPrn sl on sl.id = p.model "
                    "left join SalaryMakerPRN m on sl.maker=m.id "
                      "where c.name = :otdel "
                      "order by c.CODE";
            parentsQuery.prepare(reportQry);
            parentsQuery.bindValue(":otdel",st);
            if(!parentsQuery.exec())
                return;
            while(parentsQuery.next())
                {
                j=1;
                QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 1);
                cell->setProperty("Value", st);
                while(j<=18){
                   QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", i, j);
                   switch (j) {
                   case 1:{
                       QVariant f=parentsQuery.value(0).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 2:{
                       QVariant f=parentsQuery.value(1).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 3:{
                       QVariant f=parentsQuery.value(2).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 4:{
                       QVariant f=parentsQuery.value(3).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 5:{
                       QVariant f=parentsQuery.value(4).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 6:{
                       QVariant f=parentsQuery.value(5).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 7:{
                       QVariant f=parentsQuery.value(6).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 8:{
                       QVariant f=parentsQuery.value(7).toString();
                       cell->setProperty("Value", f);
                       break; }


                   }
                   j++;
                }
                   i++;
                }

            mExcel->setProperty("DisplayAlerts", 1);
}

void Printer::instZap()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
        int id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
        Zapchasti *zap = new Zapchasti(id,mdiAreaPrinter);
         QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(zap);
         QScreen *screen = qApp->primaryScreen();
         int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
         int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
         subWindow->setGeometry(x, y, 780, 350);
         subWindow->show();
         connect(zap,SIGNAL(Complite()),this,SLOT(msgZap()));
}

void Printer::historyZap()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    int id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    defHistory *hist = new defHistory(mdiAreaPrinter,0,id);
    QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(hist);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 580);
    subWindow->show();
}

void Printer::instKart()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();
     QModelIndex currentDiscount2  = ui->tableView->currentIndex();//получение значения первого столбца таблицы.
    int id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),2),0).toInt();
    int idP = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount2.row(),0),0).toInt();
    kartridj *kar = new kartridj(id,idP,mdiAreaPrinter);
    QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(kar);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 700, 500);
    subWindow->show();
}

void Printer::historyKart()
{

}

void Printer::history()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   QString id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toString();
    Search *poisk = new Search(mdiAreaPrinter,4,id);
    QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(poisk);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 350);
    subWindow->show();
}

void Printer::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Enter:{
                        if(ui->lineEdit->hasFocus()){
                           on_pushButtonSearch_clicked();
                        }
                        break;
                        }
    case Qt::Key_Escape:{

                        break;
                        }
}
}

void Printer::msgZap()
{
    QMessageBox::information(this,"Внимание","Запчасть установлена.",0,0);
}

void Printer::on_pushButton_hist_clicked()
{
    Search *poisk = new Search(mdiAreaPrinter,4,"");
    QMdiSubWindow *subWindow = mdiAreaPrinter->addSubWindow(poisk);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 350);
    subWindow->show();
}
