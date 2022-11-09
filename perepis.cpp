#include "perepis.h"
#include "ui_perepis.h"
#include "addperepis.h"
#include "viborotdela.h"
#include "simpledelegate.h"
#include "search.h"
#include <QProcess>
#include <QDebug>
#include <QDesktopServices>
#include <QChar>
#include <windows.h>
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QSqlError>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTextCodec>
#include <QByteArray>
#include <QAxObject>
#include <QHeaderView>
#include <QList>
#include <QKeyEvent>


//mmc.exe compmgmt.msc /computer=CoputerNameHere допилить
perepis::perepis(QSqlDatabase db, QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::perepis)
{
    mdiArea = md;
    mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation, true);
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    model = new QSqlTableModel(this,db);
    qryModel=new QSqlQueryModel(this);
    setWindowTitle("Перепись по отделам");
    ui->treeWidget->clear();
    //создание списка отделов и вывод в дерево
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
    connect(ui->tableView,SIGNAL(QAbstractItemView::currentChanged()),this,SLOT(currentChanged()));

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
         ui->comboBox->addItem("Инв № ПК содержит",QVariant(0));
         ui->comboBox->addItem("Инв № Мон. содержит",QVariant(1));
         ui->comboBox->addItem("ФИО содержит",QVariant(2));
         ui->comboBox->addItem("Кабинет",QVariant(3));
         ui->comboBox->addItem("Блокхост",QVariant(4));
         ui->comboBox->addItem("Вакансии",QVariant(5));
         ui->comboBox->addItem("Сломанные",QVariant(6));
         ui->comboBox->addItem("Не актуальные данные",QVariant(7));
         ui->comboBox->addItem("УЗ пользователя",QVariant(8));

          status = 0;



}

perepis::~perepis()
{
    delete ui;
}

void perepis::on_ButtonAdd_clicked()
{
    if(otId.value(0).toInt()==0){
        QMessageBox::warning(this,"Error","Не выбран отдел! Куда по твоему она вставится!?");
    }else{
    addPerepis *addper = new addPerepis(0,otId.value(0).toInt(),mdiArea);
    QMdiSubWindow *subWindow = mdiArea->addSubWindow(addper);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 640, 560);
    subWindow->show();
   connect(addper,SIGNAL(Reselect()),this,SLOT(reselect()));
}


}

void perepis::on_pushButtonDell_clicked()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    QString id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toString();
    if(id==0){
        QMessageBox::warning(this,"Error","Не выбрана запись!");
    }else{
    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'perepis' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
         QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {
    QSqlQuery qryDel;
    qryDel.prepare("DELETE from perepis where id=(:id);");
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
}

void perepis::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    st=item->text(0);
    QModelIndex currentDiscount  = ui->treeWidget->currentIndex();
    QString n = ui->treeWidget->model()->data(ui->treeWidget->model()->index(currentDiscount.row(),0)).toString();
    qry.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер',c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'Операционная система', p.STATUS, c.id from perepis p"
                " left join Comp c on c.id = p.COMPID"
                " left join Monitor m on m.id = p.MONITORID"
                " left join Otdel ot on ot.CODE = p.OTDELID"
                " left join OS o on o.id = c.OSid"
                " where ot.name like :nam;");
    qry.bindValue(":nam",st);
    qry.exec();
    qry.next();
    qryModel->setQuery(qry);
     ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
    ui->tableView->horizontalScrollBar();


// ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableView->hideColumn(0);

   ui->tableView->setColumnWidth(1, 60);
   ui->tableView->setColumnWidth(2, 250);
   ui->tableView->setColumnWidth(3, 130);
   ui->tableView->setColumnWidth(4, 180);
   ui->tableView->setColumnWidth(5, 120);
   ui->tableView->setColumnWidth(6, 120);
   ui->tableView->setColumnWidth(7, 120);
   ui->tableView->setColumnWidth(8, 120);
   ui->tableView->setColumnWidth(9, 120);
   ui->tableView->setColumnWidth(10, 150);
   ui->tableView->setColumnWidth(11, 80);
   ui->tableView->setColumnWidth(12, 100);
   ui->tableView->setColumnWidth(13, 80);
   ui->tableView->setColumnWidth(14, 300);

   ui->tableView->hideColumn(21);

    otId.prepare("select CODE from Otdel where name = :name;");
            otId.bindValue(":name",st);
            otId.exec();
            otId.next();
            ot=otId.value(0).toInt();
 status = 0;

 ui->label_2->clear();
 connect(ui->tableView->selectionModel(),SIGNAL(currentChanged(QModelIndex const&,QModelIndex const&)),this,SLOT(curentActiv(QModelIndex)));

}

void perepis::on_tableView_doubleClicked(const QModelIndex &index)
{
        //переменная статус для того чтобы взять нужный номер отдела если редактируешь через поиск
    if(status!=1){
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'perepis' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
   addPerepis *addper = new addPerepis(id,ot,mdiArea);
   QMdiSubWindow *subWindow = mdiArea->addSubWindow(addper);
   QScreen *screen = qApp->primaryScreen();
   int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
   int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
   subWindow->setGeometry(x, y, 640, 560);
   subWindow->show();
   connect(addper,SIGNAL(Reselect()),this,SLOT(reselect()));
   QSqlQuery log;
   log.prepare("exec addLog :tbl, :link;");
   log.bindValue(":tbl","perepis");
   log.bindValue(":link",id);
   log.exec();
   }
    }
    else{
        QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
       id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
       ot = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),21),0).toInt();
       QSqlQuery log_prov;
       log_prov.prepare("select user_name from JURNAL where tbl = 'perepis' AND Link = :link;");
       log_prov.bindValue(":link",id);
       log_prov.exec();
       log_prov.next();
       if(log_prov.value(0).toString().length()>0){
            QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
       }else {
       addPerepis *addper = new addPerepis(id,ot,mdiArea);
       QMdiSubWindow *subWindow = new QMdiSubWindow;//mdiArea->addSubWindow(addper);
       subWindow->setWidget(addper);
       mdiArea->addSubWindow(subWindow);
       QScreen *screen = qApp->primaryScreen();
       int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
       int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
       subWindow->setGeometry(x, y, 640, 560);
       subWindow->setMaximumSize(645,565);
       subWindow->setMinimumSize(640,560);
       subWindow->show();
       connect(addper,SIGNAL(Reselect()),this,SLOT(reselect()));
       QSqlQuery log;
       log.prepare("exec addLog :tbl, :link;");
       log.bindValue(":tbl","perepis");
       log.bindValue(":link",id);
       log.exec();
       }
    }
}

void perepis::reselect()
{
    if(status!=1){
    qry.exec();
    qry.next();
    qryModel->setQuery(qry);
    }else {
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
}
}

void perepis::on_pushButton_clicked()  // Поиск по инв ПК И МОН
{
    switch (ui->comboBox->currentIndex()) {
    case 0:{
       searchPC.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер', "
                        "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                        "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'операционная система', p.STATUS, p.OTDELID,ot.name from perepis p "
                        "left join Comp c on c.id = p.COMPID "
                        "left join Monitor m on m.id = p.MONITORID "
                        "left join Otdel ot on ot.CODE = p.OTDELID "
                        "left join OS o on o.id = c.OSid "
                        "where c.inv like :inv");
       searchPC.bindValue(":inv","%"+ui->lineEdit->text()+"%");
       searchPC.exec();
       searchPC.next();
       qryModel->setQuery(searchPC);
         ui->tableView->hideColumn(0);
          ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;
    }
    case 1:{
        searchPC.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер', "
                         "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                         "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'операционная система',p.STATUS,p.OTDELID,ot.name from perepis p "
                         "left join Comp c on c.id = p.COMPID "
                         "left join Monitor m on m.id = p.MONITORID "
                         "left join Otdel ot on ot.CODE = p.OTDELID "
                         "left join OS o on o.id = c.OSid "
                         "where m.inv like :inv");
        searchPC.bindValue(":inv","%"+ui->lineEdit->text()+"%");
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
          ui->tableView->hideColumn(0);
           ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;
    }
    case 2:{
        searchPC.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер', "
                         "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                         "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'операционная система',p.STATUS,p.OTDELID,ot.name from perepis p "
                         "left join Comp c on c.id = p.COMPID "
                         "left join Monitor m on m.id = p.MONITORID "
                         "left join Otdel ot on ot.CODE = p.OTDELID "
                         "left join OS o on o.id = c.OSid "
                         "where p.USERNAM like :inv;");
        //QString s = "'%"+ui->lineEdit->text()+"%'";
        searchPC.bindValue(":inv","%"+ui->lineEdit->text()+"%");
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
          ui->tableView->hideColumn(0);
           ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;
    }
    case 3:{
        searchPC.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер', "
                         "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                         "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'операционная система',p.STATUS,p.OTDELID,ot.name from perepis p "
                         "left join Comp c on c.id = p.COMPID "
                         "left join Monitor m on m.id = p.MONITORID "
                         "left join Otdel ot on ot.CODE = p.OTDELID "
                         "left join OS o on o.id = c.OSid "
                         "where p.KAB like :kab;");
        //QString s = "'%"+ui->lineEdit->text()+"%'";
        searchPC.bindValue(":kab",ui->lineEdit->text());
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
          ui->tableView->hideColumn(0);
           ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;

    }
    case 4:{
        search="select ot.CODE,ot.name,p.KAB, p.USERNAM, p.USERUZ, p.TEL, p.ROZTEL, p.ROZPC, c.name, CONCAT(c.maker,' ',c.model,' ') as 'Компьютер', "
               "c.inv,c.ser as 'серийный номер',c.hddZ,c.ramZ,CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv,m.ser, c.eToken, p.COMMENT from perepis p "
               "left join Comp c on c.id = p.COMPID "
               "left join Monitor m on m.id = p.MONITORID "
               "left join Otdel ot on ot.CODE = p.OTDELID "
               "left join OS o on o.id = c.OSid "
               "where c.blokhost like :bh;";

        searchPC.prepare(search);
        //QString s = "'%"+ui->lineEdit->text()+"%'";
        searchPC.bindValue(":bh",ui->lineEdit->text());
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
          ui->tableView->hideColumn(0);
           ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;

    }
    case 5:{
        searchPC.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер', "
                         "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                         "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'операционная система',p.STATUS,p.OTDELID,ot.name from perepis p "
                         "left join Comp c on c.id = p.COMPID "
                         "left join Monitor m on m.id = p.MONITORID "
                         "left join Otdel ot on ot.CODE = p.OTDELID "
                         "left join OS o on o.id = c.OSid "
                         "where p.USERNAM like '%вакансия%';");

        searchPC.bindValue(":bh",ui->lineEdit->text());
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
          ui->tableView->hideColumn(0);
           ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;

    }
    case 6:{
        searchPC.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер', "
                         "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                         "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'операционная система',p.STATUS,p.OTDELID,ot.name from perepis p "
                         "left join Comp c on c.id = p.COMPID "
                         "left join Monitor m on m.id = p.MONITORID "
                         "left join Otdel ot on ot.CODE = p.OTDELID "
                         "left join OS o on o.id = c.OSid "
                         "where p.STATUS='2';");

        //searchPC.bindValue(":bh",ui->lineEdit->text());
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
          ui->tableView->hideColumn(0);
           ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;

    }
    case 7:{
        searchPC.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер', "
                         "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                         "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'операционная система',p.STATUS,p.OTDELID,ot.name from perepis p "
                         "left join Comp c on c.id = p.COMPID "
                         "left join Monitor m on m.id = p.MONITORID "
                         "left join Otdel ot on ot.CODE = p.OTDELID "
                         "left join OS o on o.id = c.OSid "
                         "where p.STATUS='1';");

        //searchPC.bindValue(":bh",ui->lineEdit->text());
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
          ui->tableView->hideColumn(0);
           ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;

    }
    case 8:{
        searchPC.prepare("select p.id,p.KAB as 'Кабинет', p.USERNAM as 'Пользователь', p.USERUZ as 'Учетная запись',CONCAT(c.maker,' ',c.model,' ') as 'Компьютер',c.name as 'Имя',c.inv as 'инвентарный номер', "
                         "c.ser as 'серийный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер',m.ser as 'серийный номер', p.ROZPC as 'Розетка ПК', p.TEL as 'номер телефона', p.ROZTEL as 'Розетка Тел', p.COMMENT as 'Комментарий', "
                         "c.spisan,c.blokhost,m.spisan,c.eToken,CONCAT(o.name,' ',o.razr,'x') as 'операционная система',p.STATUS,p.OTDELID,ot.name from perepis p "
                         "left join Comp c on c.id = p.COMPID "
                         "left join Monitor m on m.id = p.MONITORID "
                         "left join Otdel ot on ot.CODE = p.OTDELID "
                         "left join OS o on o.id = c.OSid "
                         "where p.USERUZ like :uz;");

        //searchPC.bindValue(":bh",ui->lineEdit->text());
        searchPC.bindValue(":uz","%"+ui->lineEdit->text()+"%");
        searchPC.exec();
        searchPC.next();
        qryModel->setQuery(searchPC);
          ui->tableView->hideColumn(0);
           ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
        status = 1;
        break;

    }
    }
}

void perepis::ping()
{

    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   QString nam = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),5),0).toString();
   QByteArray out;
   QTextCodec *codec = QTextCodec::codecForName("cp-866");

    QProcess *ping = new QProcess(0);
    QString exec = "ping";
    QStringList param;
    param<<"-n "<<"1 "<<nam;
    ping->start(exec,param);
    if(!ping->waitForFinished()){}else{

        out=ping->readAllStandardOutput();
         QString st = codec->toUnicode(out.data());
        QMessageBox::information(this,"Состояние",st);
         delete ping;}


}



void perepis::openExpl()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   QString nam = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),5),0).toString();
    QFileDialog diag;
    diag.setDirectory("//"+nam+"/c$");
    diag.setFileMode(QFileDialog::AnyFile);
    diag.exec();


}


void perepis::customMenu(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *op = new QAction(tr("открыть \C$"),this);
    connect(op,SIGNAL(triggered()),this,SLOT(openExpl()));
    menu->addAction(op);
    //menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    QAction *ping = new QAction(tr("Проверить доступность"),this);
    connect(ping,SIGNAL(triggered()),this,SLOT(ping()));
    menu->addAction(ping);

    QAction *st = new QAction(tr("Установить/снять статус Не актуальные данные"),this);
    connect(st,SIGNAL(triggered()),this,SLOT(editSt()));
    menu->addAction(st);

    QAction *teh = new QAction(tr("Установить/снять статус сломан"),this);
    connect(teh,SIGNAL(triggered()),this,SLOT(StTehSost()));
    menu->addAction(teh);

    QAction *his = new QAction(tr("История записи"),this);
    connect(his,SIGNAL(triggered()),this,SLOT(History()));
    menu->addAction(his);

    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

}

void perepis::on_pushButton_2_clicked() //перемещение из отдела в отдел
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   if(id==0){
       QMessageBox::warning(this,"Error","Не выбрана запись!!Как у вас мозгов хватило тыкнуть на кнопку переноса не выбрав запись!?");
   }else{
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'perepis' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
   ViborOtdela *vib = new ViborOtdela(mdiArea,id,0);
   QMdiSubWindow *subWindow = mdiArea->addSubWindow(vib);
   QScreen *screen = qApp->primaryScreen();
   int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
   int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
   subWindow->setGeometry(x, y, 370, 460);
   subWindow->show();
   connect(vib,SIGNAL(Reselect()),this,SLOT(reselect()));
   }
}
}

void perepis::on_Report_Button_clicked()
{

    if(status==1){
        report();
    }else{
    QString tmp_s=  QApplication::applicationDirPath();
        // получаем указатель на Excel
            QAxObject *mExcel = new QAxObject("Excel.Application",this);
            //видимость
            mExcel->dynamicCall( "SetVisible(bool)", true );
            // на книги
            QAxObject *workbooks = mExcel->querySubObject("Workbooks");
            // на директорию, откуда грузить книг
            QAxObject *workbook = workbooks->querySubObject( "Open(const QString&)",tmp_s+ "/perepis21.xltx" );
            // на листы
            QAxObject *mSheets = workbook->querySubObject( "Sheets" );
            // указываем, какой лист выбрать
            QAxObject *StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant("list") );
            int i,j;
            i=3;
            QSqlQuery parentsQuery;
            reportQry="select o.CODE,o.name, p.KAB, p.USERNAM, p.USERUZ, p.TEL, p.ROZTEL, p.ROZPC, c.name, CONCAT(c.maker,' ',c.model,' ') as 'Компьютер', "
                      "c.inv,c.ser as 'серийный номер',c.hddZ,c.ramZ,CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv,m.ser, c.eToken, p.COMMENT FROM perepis p "
                      "inner join Otdel o on p.OTDELID=o.CODE "
                      "inner join Comp c on p.COMPID=c.id "
                      "inner join Monitor m on p.MONITORID = m.id "
                      "where o.name = :otdel "
                      "order by CODE;";
            if(ui->comboBox->currentIndex()==4 && status==1){//----------------------------------------------------
                reportQry=search;
                parentsQuery.prepare(reportQry);
                parentsQuery.bindValue(":bh",ui->lineEdit->text());
                qDebug()<<"st=1";
            }else{
            parentsQuery.prepare(reportQry);
            parentsQuery.bindValue(":otdel",st);

            }//-------------------------------------------------------------------------------
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
                       QVariant f=parentsQuery.value(2).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 2:{
                       QVariant f=parentsQuery.value(3).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 3:{
                       QVariant f=parentsQuery.value(4).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 4:{
                       QVariant f=parentsQuery.value(5).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 5:{
                       QVariant f=parentsQuery.value(6).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 6:{
                       QVariant f=parentsQuery.value(7).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 7:{
                       QVariant f=parentsQuery.value(8).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 8:{
                       QVariant f=parentsQuery.value(9).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 9:{
                       QVariant f=parentsQuery.value(10).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 10:{
                       QVariant f=parentsQuery.value(11).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 11:{
                       QVariant f=parentsQuery.value(12).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 12:{
                       QVariant f=parentsQuery.value(13).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 13:{
                       QVariant f=parentsQuery.value(14).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 14:{
                       QVariant f=parentsQuery.value(15).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 15:{
                       QVariant f=parentsQuery.value(16).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 16:{
                       QVariant f=parentsQuery.value(17).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 17:{
                       QVariant f=parentsQuery.value(18).toString();
                       cell->setProperty("Value", f);
                       break; }



                   }
                   j++;
                }
                   i++;
                }

            mExcel->setProperty("DisplayAlerts", 1);
}
}

void perepis::editSt() //статсус
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    QString id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toString();
    int st = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),20),0).toInt();
    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'perepis' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
         QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {

        if(st == 0){
    QSqlQuery qryDel;
    qryDel.prepare("UPDATE perepis SET STATUS = :st where id=(:id);");
      qryDel.bindValue(":st",1);
    qryDel.bindValue(":id",id);
        if(qryDel.exec()){
          reselect();
        }else{
           QMessageBox::about(this,"Error",qryDel.lastError().text());
        }
        }else if(st==1){
            QSqlQuery qryDel;
            qryDel.prepare("UPDATE perepis SET STATUS = :st where id=(:id);");
              qryDel.bindValue(":st",0);
            qryDel.bindValue(":id",id);
                if(qryDel.exec()){
                    reselect();
                }else{
                   QMessageBox::about(this,"Error",qryDel.lastError().text());
                }
        }
    }
}

void perepis::StTehSost() //состояния
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    QString id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toString();
    int st = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),20),0).toInt();
    int comp = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),21),0).toInt();
    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'perepis' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
         QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {

        if(st == 0 || st==1){

    QSqlQuery qryDel;
    qryDel.prepare("UPDATE perepis SET STATUS = :st where id=(:id); UPDATE Comp SET STATUS = :st where id = :comp;");
      qryDel.bindValue(":st",2);
    qryDel.bindValue(":id",id);
    qryDel.bindValue(":comp",comp);
        if(qryDel.exec()){
             reselect();
        }else{
           QMessageBox::about(this,"Error",qryDel.lastError().text());
        }
        }else if(st==2){
            QSqlQuery qryDel;
            qryDel.prepare("UPDATE perepis SET STATUS = :st where id=(:id);UPDATE Comp SET STATUS = :st where id = :comp;");
              qryDel.bindValue(":st",0);
            qryDel.bindValue(":id",id);
             qryDel.bindValue(":comp",comp);
                if(qryDel.exec()){
                     reselect();
                }else{
                   QMessageBox::about(this,"Error",qryDel.lastError().text());
                }
        }
    }
}

void perepis::History()//история записи
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   QString id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toString();
    Search *poisk = new Search(mdiArea,3,id);
    QMdiSubWindow *subWindow = mdiArea->addSubWindow(poisk);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 350);
    subWindow->show();
}


void perepis::curentActiv(const QModelIndex &index) //Подвальчик
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id2 = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QList<QString> statusList = {"- в рабочем состоянии","- не актуальные данные","- cломан"};
   qry1.prepare("select c.name as 'Имя',c.inv as 'инвентарный номер',CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv as 'инвентарный номер', p.COMMENT as 'Комментарий', "
               "c.spisan,m.spisan,CONCAT(o.name,' ',o.razr,'x') as 'Операционная система', p.STATUS, c.ramZ from perepis p"
               " left join Comp c on c.id = p.COMPID"
               " left join Monitor m on m.id = p.MONITORID"
               " left join Otdel ot on ot.CODE = p.OTDELID"
               " left join OS o on o.id = c.OSid"
               " where p.id = :id;");
   qry1.bindValue(":id",id2);
   qry1.exec();
   qry1.next();

   ui->label_2->setText("Компьютер  "+qry1.value(0).toString()+"  "+qry1.value(1).toString()+"  Кол-во ram: "+qry1.value(9).toString()+"  OS: "+qry1.value(7).toString()+"\nМонитор "+qry1.value(2).toString()+"  "+qry1.value(3).toString()+" \n"+qry1.value(4).toString()+"\nСтатус  "+statusList[qry1.value(8).toInt()]);
}

void perepis::keyPressEvent(QKeyEvent *e)
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

void perepis::on_comboBox_activated(int index)
{
    if(index==4){
            ui->lineEdit->setEnabled(true);
        ui->lineEdit->setPlaceholderText("1-есть, 0 - нет");
    }else if(index==5){
        ui->lineEdit->setEnabled(false);
    }
    else if(index==6){
    ui->lineEdit->setEnabled(false);
    }
    else if(index==7){
    ui->lineEdit->setEnabled(false);
    }
    else {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit->setPlaceholderText("");
            }
}

void perepis::on_HistoryButton_clicked()
{
    Search *poisk = new Search(mdiArea,3,"");
    QMdiSubWindow *subWindow = mdiArea->addSubWindow(poisk);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 670, 350);
    subWindow->show();
}

void perepis::report()
{
    QString tmp_s=  QApplication::applicationDirPath();
        // получаем указатель на Excel
            QAxObject *mExcel = new QAxObject("Excel.Application",this);
            //видимость
            mExcel->dynamicCall( "SetVisible(bool)", true );
            // на книги
            QAxObject *workbooks = mExcel->querySubObject("Workbooks");
            // на директорию, откуда грузить книг
            QAxObject *workbook = workbooks->querySubObject( "Open(const QString&)",tmp_s+ "/reportS.xltx" );
            // на листы
            QAxObject *mSheets = workbook->querySubObject( "Sheets" );
            // указываем, какой лист выбрать
            QAxObject *StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant("list") );

            int rows = ui->tableView->horizontalHeader()->count();
            int columns = ui->tableView->verticalHeader()->count();
            QString rep = ui->comboBox->currentText()+"   "+ui->lineEdit->text();
            QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 1);
            cell->setProperty("Value", rep);

            for(int k=1; k<rows; k++){  //заголовок
                QVariant header = ui->tableView->model()->headerData(k,Qt::Horizontal);
                QAxObject *cell = StatSheet->querySubObject("Cells(QVariant,QVariant)",2,k);
                cell->setProperty("Value",QVariant(header));
                delete cell;
            }

            for(int i =3; i<columns+3; i++){
                for(int j=1; j<rows; j++){
                    QVariant value = ui->tableView->model()->data(ui->tableView->model()->index(i-3,j));
                    QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", i, j);
                    cell->setProperty("Value", QVariant(value));
                    value = "";
                }
            }




            mExcel->setProperty("DisplayAlerts", 1);
            delete StatSheet;
            delete mSheets;
            delete workbook;
            delete workbooks;
            delete mExcel;

}
