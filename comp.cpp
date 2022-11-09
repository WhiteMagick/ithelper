#include "comp.h"
#include "ui_comp.h"
#include "addcomp.h"
#include "login.h"
#include "delegateforcomp.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlError>
#include <QInputDialog>
#include <QDir>
#include <QMenu>
#include <QClipboard>
#include <QAxObject>
#include <QUuid>
#include <qt_windows.h>
#include <QStringList>
#include <QKeyEvent>

Comp::Comp(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Comp)
{

    MdiAreaComp = md;
    ui->setupUi(this);   
    setWindowTitle("Список компьютеров");
//    zap1 = new Zapros();
//    thread_1 = new QThread(this);
    zap2 = new Zapros();
    thread_2 = new QThread(this);
//    //------------------------------------------соединеие с потоком----------------------------------------------------------------------------
//    connect(this,SIGNAL(setStr(QString)),zap1,SLOT(setStr(QString)));
//    connect(zap1, &Zapros::finished, thread_1, &QThread::terminate);
//    connect(zap1,SIGNAL(send(QSqlQuery)),this,SLOT(getQry(QSqlQuery)),Qt::DirectConnection);
//    zap1->moveToThread(thread_1);
    connect(zap2, &Zapros::finished, thread_2, &QThread::terminate);
    connect(this,SIGNAL(send(int)),this, SLOT(message(int)));
    connect(zap2,SIGNAL(sendWMI(QStringList,int)),this,SLOT(otvetWMI(QStringList,int)),Qt::DirectConnection);
    connect(this,SIGNAL(setID(QString)),zap2,SLOT(setName(QString)));
    connect(this,SIGNAL(setID2(QString)),zap2,SLOT(setName2(QString)));
    zap2->moveToThread(thread_2);
    //-----------------------------------------------------------------------------------------------------------------------------------------

    ui->progressBar->setVisible(false);
   qryModel=new QSqlQueryModel(this);


    ui->comboBox->clear();
    ui->comboBox->addItem("Инв № содержит",QVariant(0));
    ui->comboBox->addItem("Кол-во ОЗУ",QVariant(1));
    ui->comboBox->addItem("Объём HDD",QVariant(2));
    ui->comboBox->addItem("Операционная система",QVariant(3));
    ui->comboBox->addItem("БлокХост",QVariant(4));
    ui->comboBox->addItem("eToken содержит",QVariant(6));
    ui->comboBox->addItem("Статус списания",QVariant(7));
    ui->comboBox->addItem("Производитель",QVariant(8));
    ui->comboBox->addItem("Год выпуска",QVariant(9));

    qry.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, comment,STATUS,year from Comp;");
    qry.exec();
    qryModel->setQuery(qry);
    qryModel->setHeaderData(1, Qt::Horizontal, "Производитель");
    qryModel->setHeaderData(2, Qt::Horizontal, "Модель");
    qryModel->setHeaderData(3, Qt::Horizontal, "Имя");
    qryModel->setHeaderData(4, Qt::Horizontal, "Инвентарный номер");
    qryModel->setHeaderData(5, Qt::Horizontal, "Серийный номер");
    qryModel->setHeaderData(6, Qt::Horizontal, "Процессор");
    qryModel->setHeaderData(7, Qt::Horizontal, "кол-во Ghz");
    qryModel->setHeaderData(8, Qt::Horizontal, "Оперативная парять");
    qryModel->setHeaderData(9, Qt::Horizontal, "кол-во Gb");
    qryModel->setHeaderData(10, Qt::Horizontal, "Жесткий диск");
    qryModel->setHeaderData(11, Qt::Horizontal, "кол-во GB");
    qryModel->setHeaderData(12, Qt::Horizontal, "ОС");
    qryModel->setHeaderData(13, Qt::Horizontal, "eToken");
    qryModel->setHeaderData(14, Qt::Horizontal, "IP");
    qryModel->setHeaderData(15, Qt::Horizontal, "Списан");
    qryModel->setHeaderData(16, Qt::Horizontal, "БлокХост");
    qryModel->setHeaderData(17, Qt::Horizontal, "Комментарий");
    qryModel->setHeaderData(18, Qt::Horizontal, "Год выпуска");
     ui->tableView->setModel(qryModel);
      ui->tableView->setItemDelegate(new DelegateForComp(ui->tableView));
     ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
     connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
     ui->tableView->horizontalScrollBar();
     ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


}

Comp::~Comp()
{
    delete ui;
}

void Comp::getQry(QSqlQuery qry)
{

}

void Comp::select1()
{
    s= "select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, comment from Comp;";  
    thread_1->start();
    emit setStr(s);


}

void Comp::on_ButtonAdd_clicked()
{

    AddComp *comp = new AddComp(0,MdiAreaComp);
    QMdiSubWindow *subWindow = MdiAreaComp->addSubWindow(comp);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 1031, 320);
    subWindow->show();
     connect(comp,SIGNAL(Reselect()),this,SLOT(reselect()));


}

void Comp::on_ButtonDell_clicked()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'Comp' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    QSqlQuery qryDelSubdiv;
    qryDelSubdiv.prepare("DELETE from Comp where id=(:id);");
    qryDelSubdiv.bindValue(":id",id);
    QMessageBox::StandardButton msg;
    msg=QMessageBox::question(this,"Внимание","Удаление записи возможно только в том случае если запись не связана с другими таблицами.",QMessageBox::Yes|QMessageBox::No);
    if(msg == QMessageBox::Yes){
        if(qryDelSubdiv.exec()){
            qryDelSubdiv.next();
             reselect();
        }else{
           QMessageBox::about(this,"Error",qryDelSubdiv.lastError().text());
        }
    }
   }

}

void Comp::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   QSqlQuery log_prov;
   log_prov.prepare("select user_name from JURNAL where tbl = 'Comp' AND Link = :link;");
   log_prov.bindValue(":link",id);
   log_prov.exec();
   log_prov.next();
   if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
   }else {
    AddComp *comp = new AddComp(id,MdiAreaComp);
    QMdiSubWindow *subWindow = MdiAreaComp->addSubWindow(comp);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 1031, 340);
    subWindow->show();
    connect(comp,SIGNAL(Reselect()),this,SLOT(reselect()));
    QSqlQuery log;
    log.prepare("exec addLog :tbl, :link;");
    log.bindValue(":tbl","Comp");
    log.bindValue(":link",id);
    log.exec();
   }
}

void Comp::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Enter:{
                        if(ui->lineEdit->hasFocus()){
                           on_pushButton_2_clicked();
                        }
                        break;
                        }
    case Qt::Key_Escape:{

                        break;
                        }
}
}

void Comp::reselect()
{

        search.exec();
        qryModel->setQuery(search);


   // qDebug()<<"reselect";

}

void Comp::insertWmi()
{

    if(wmi.at(0)!="-"){
    qryUp.prepare("UPDATE Comp SET CPU=:CPU, "
                  "HDD=:HDD, IP=:IP,ramZ=:ramZ,hddZ=:hddZ,cpuZ=:cpuZ WHERE id=:id;");
    qryUp.bindValue(":id",PC);
    qryUp.bindValue(":CPU",wmi.at(0));
    qryUp.bindValue(":HDD",wmi.at(1));
    qryUp.bindValue(":IP",wmi.at(2));
    qryUp.bindValue(":ramZ",wmi.at(3));
    qryUp.bindValue(":hddZ",wmi.at(4));
    qryUp.bindValue(":cpuZ",wmi.at(5));
    qryUp.exec();
    wmi.clear();
    emit send(0);
     zap2->finished();
    }else{
        qDebug()<<"не удалось считать данные!";
       wmi.clear();
       zap2->finished();
       emit send(1);
    }
}

void Comp::insertWmi2()
{
    if(wmi2.at(0)!="-"){
    qryUp.prepare("UPDATE Comp SET CPU=:CPU, "
                  "HDD=:HDD, IP=:IP,ramZ=:ramZ,hddZ=:hddZ,cpuZ=:cpuZ WHERE id=:id;");
    qryUp.bindValue(":id",PC);
    qryUp.bindValue(":CPU",wmi2.at(0));
    qryUp.bindValue(":HDD",wmi2.at(1));
    qryUp.bindValue(":IP",wmi2.at(2));
    qryUp.bindValue(":ramZ",wmi2.at(3));
    qryUp.bindValue(":hddZ",wmi2.at(4));
    qryUp.bindValue(":cpuZ",wmi2.at(5));
    qryUp.exec();
    wmi2.clear();
     zap2->finished();
    }else{
        qDebug()<<"не удалось считать данные!";
       wmi2.clear();
       zap2->finished();

    }

}



void Comp::on_pushButton_clicked()  //Копирование записией
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();

   bool ok;
   int kol = QInputDialog::getInt(this,tr("Сколько копий"),tr("кол-во"),1,0,100,1,&ok);
   if(ok){
       for(kol;kol>0;kol--){

              copy.prepare("exec copy :id,:name,:inv,:ser;");
              copy.bindValue(":id",id);
              copy.bindValue(":name"," ");
              copy.bindValue(":inv","б/н"+QString::number(kol));
              copy.bindValue(":ser","б/н"+QString::number(kol));
              if(copy.exec()){

              }

       }
   }

    reselect();

}

void Comp::on_pushButton_2_clicked() // поиск
{
    //filter = 1;
    switch (ui->comboBox->currentIndex()) {
    case 0:{

        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where inv like :inv;");
        search.bindValue(":inv","%"+ui->lineEdit->text()+"%");
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 1:{
        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where ramZ = :ramZ;");
        search.bindValue(":ramZ",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 2:{
        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where hddZ = :hddZ;");
        search.bindValue(":hddZ",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 3:{

        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where OSid = :OSid;");
        search.bindValue(":OSid",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 4:{

        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where blokhost = :blokhost;");
        search.bindValue(":blokhost",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 5:{

        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where eToken like :eToken;");
        search.bindValue(":eToken","%"+ui->lineEdit->text()+"%");
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 6:{

        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where spisan = :spisan;");
        search.bindValue(":spisan",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 7:{

        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where maker like :maker;");
        search.bindValue(":maker","%"+ui->lineEdit->text()+"%");
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    case 8:{

        search.prepare("select id, maker, model, name, inv, ser, CPU, cpuZ, RAM, ramZ, HDD, hddZ, OSid, eToken, IP, spisan, blokhost, year, comment from Comp where year= :year;");
        search.bindValue(":year",ui->lineEdit->text());
        search.exec();
        search.next();
        qryModel->setQuery(search);
        break;
    }
    }
}

void Comp::on_comboBox_activated(int index)
{
    if(index==3){
        ui->lineEdit->setPlaceholderText("Введите id ОС для поиска");
    }else if (index==4) {
        ui->lineEdit->setPlaceholderText("1-есть, 0 - нет");
    }else if (index==6) {
        ui->lineEdit->setPlaceholderText("0-не списан, 1-списан, 2-на списании");
    }
    else {
        ui->lineEdit->setPlaceholderText("");
            }

}

void Comp::on_pushButton_3_clicked()
{

     qry.exec();
     qryModel->setQuery(qry);

}

void Comp::copyfromBuff()
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

void Comp::customMenu(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *op = new QAction(tr("Копировать"),this);
    QAction *wmi = new QAction(tr("Заполнить данными по WMI"),this);
    connect(op,SIGNAL(triggered()),this,SLOT(copyfromBuff()));
    connect(wmi,SIGNAL(triggered()),this,SLOT(wmiselect()));
    menu->addAction(op);
     menu->addAction(wmi);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void Comp::wmiselect()  //Запросы к WMI для заполнеия полей в карточке ПК
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    QString id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),3),0).toString();
    PC = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    if(thread_2->isRunning()){
        QMessageBox::about(this,"Error","Дождитесь окончания предыдущего запроса.");
    }else{
   //qDebug()<<"go";
    thread_2->start();
    emit setID(id);
    }


}

void Comp::otvetWMI(QStringList slist,int fl)
{

    
    switch (fl) {
    case 0:{
        wmi = (QStringList()<<slist.at(0)<<slist.at(1)<<slist.at(2)<<slist.at(3)<<slist.at(4)<<slist.at(5));

        for (int i;i<slist.size();i++) {
         qDebug()<< slist.at(i) <<endl <<i;
        }
        insertWmi();
        break;
    }
    case 1: {
        wmi = (QStringList()<<slist.at(0)<<slist.at(1)<<slist.at(2)<<slist.at(3)<<slist.at(4)<<slist.at(5));

        for (int i;i<slist.size();i++) {
         qDebug()<< slist.at(i) <<endl <<i;
        }
        insertWmi2();
        break;
    }
    }


}

void Comp::message(int i)
{
    switch (i) {
    case 0:{
        qry.exec();
        qryModel->setQuery(qry);
        //QMessageBox::about(this,"Внимание","Данные загруженны");
        QMessageBox msgBox;
          msgBox.setText("Данные загруженны.");
          msgBox.exec();
        //zap2->finished();
       // qDebug("use");

        break;
    }
    case 1: {
         QMessageBox::about(this,"Error","Не удалось считать данные!");
        break;
    }
    }


}

void Comp::on_wmiFill_clicked()
{
    QSqlQuery count;
    count.prepare("select count(name)from Comp;");
    count.exec();
    count.next();
    ui->progressBar->setMaximum(count.value(0).toInt());
    ui->progressBar->setVisible(true);
    progress=0;

    QString sSql = QString("select name,id from Comp;");
        QSqlQuery parentsQuery;
        if(!parentsQuery.exec(sSql))
            return;
        while(parentsQuery.next())
        {
            int err=0; // для выхода из цикла запросов к wmi. После первого ошибочного.
             int c=0; //для прербора запросов к wmi
            ozu=0;
            sizeHDD=0;
            cpuz=0;
            qDebug()<<parentsQuery.value(0).toString();
            bool ok;
              HRESULT hr = CoInitialize(0);

             while(c<=3){
                  if(err==0){
             if(c==0){
            query = QString("SELECT * FROM Win32_DiskDrive");
             }
             else if(c==1){
             query = QString("SELECT * FROM Win32_Processor");
             }
             else if(c==2){
             query = QString("SELECT * FROM Win32_IP4RouteTable");
             }
             else if(c==3){
             query = QString("SELECT * FROM Win32_PhysicalMemory");
             }

             qDebug()<<"c="<<c;

              try {

                  QAxObject *objWebmLocator = new QAxObject("WbemScripting.SWbemLocator");
                  if(!objWebmLocator){
                      qDebug()<<"WebmScripting.SWbemLocator not created";
                  }
                  QAxObject *objWMIService = objWebmLocator->querySubObject(
                              "ConnectServer(QString&,QString&)",
                              QString(parentsQuery.value(0).toString()+".regions.tax.nalog.ru"),QString("root\\cimv2"));
                  if(!objWMIService){
                      qDebug("objWMIService not created");
                      err=1;
                      throw objWebmLocator->isNull();
                      delete objWebmLocator;
                  }//Win32_DiskDrive   Win32_Processor  Win32_IP4RouteTable

                  QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))",query);
                  if(!objInterList){
                      qDebug("Answer from ExecQuery is null");
                      err=1;
                      throw objWebmLocator->isNull();
                      delete objWebmLocator;
                  }
                  if(objInterList->dynamicCall("count").toInt()==0){
                      qDebug("Count is 0");
                      delete objWebmLocator;
                  }

                  QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
                  if(!enum1){
                      qDebug("Query _NewEnum failed");
                      err=1;
                      throw objWebmLocator->isNull();
                      delete objWebmLocator;
                  }
                  IEnumVARIANT *enumInterface = 0;
                  enum1->queryInterface(IID_IEnumVARIANT,(void**)&enumInterface);
                  if(!enumInterface){
                      qDebug("Query interface failed");
                      err=1;
                      throw objWebmLocator->isNull();
                       delete objWebmLocator;
                  }
                  enumInterface->Release();
                  QAxObject *item = 0;
                  for(int i = 0; i < objInterList->dynamicCall("Count").toInt(); i++){ //цикл перебора результатов запроса.
                      VARIANT *theItem = (VARIANT*)malloc(sizeof (VARIANT));
                      if(enumInterface->Next(1,theItem,nullptr)==S_FALSE){
                          qDebug("enum next failed");
                          delete theItem;
                           delete objWebmLocator;
                      }
                      item = new QAxObject((IUnknown*)theItem->punkVal);
                      if(!item){
                          qDebug("getting result item failed");
                          delete theItem;
                           delete objWebmLocator;
                      }

                      if(c==0){
                          QString usb=item->dynamicCall("PNPDeviceID").toString();
                          if(!usb.contains("USBSTOR")){
                      hdd = item->dynamicCall("Model").toString();
                      sizeHDD+=item->dynamicCall("size").toLongLong();
                               }

                      }
                      else if(c==1 && i==0){
                         cpu= item->dynamicCall("Name").toString();
                        cpuz=item->dynamicCall("MaxClockSpeed").toInt();


                      }else if(c==2 && i==2){
                        ip= item->dynamicCall("Name").toString();

                      }
                      else if (c==3) {

                         LONGLONG oz= item->dynamicCall("Capacity").toLongLong();
                         ozu=ozu+oz;


                      }
                       delete theItem;
                  }
                   delete objWebmLocator;



              } catch (...) {


              }
             c++;
                  }else{
                      c=4;
                      qDebug()<<"exit";
                  }

          }
             //кусочек хрени. почему-то не в одну строку из longlong в double не переводится.
              double ozu1=ozu/1024;
             double ozu2=ozu1/1024;
             double ozu3=ozu2/1024;
             double sizeH = sizeHDD/1024/1024/1024;
             QStringList list2;
             wmi2 <<cpu <<hdd <<ip <<QString::number(ozu3) <<QString::number(sizeH) <<QString::number(cpuz/1000);
              if(wmi2.at(0)==""){
                  qDebug()<<"пусто";
                  for (int i;i<wmi2.size();i++) {
                   qDebug()<< wmi2.at(i) <<endl <<i;
                  }
                  wmi2.clear();
                 cpu.clear();
                 hdd.clear();
                 ip.clear();

              }else{
             qDebug()<<"не пусто";
             for (int i;i<wmi2.size();i++) {
              qDebug()<< wmi2.at(i) <<endl <<i;
             }
             qryUp.prepare("UPDATE Comp SET CPU=:CPU, "
                           "HDD=:HDD, IP=:IP,ramZ=:ramZ,hddZ=:hddZ,cpuZ=:cpuZ WHERE id=:id;");
             qryUp.bindValue(":id",parentsQuery.value(1).toString());
             qryUp.bindValue(":CPU",wmi2.at(0));
             qryUp.bindValue(":HDD",wmi2.at(1));
             qryUp.bindValue(":IP",wmi2.at(2));
             qryUp.bindValue(":ramZ",wmi2.at(3));
             qryUp.bindValue(":hddZ",wmi2.at(4));
             qryUp.bindValue(":cpuZ",wmi2.at(5));
             if(qryUp.exec()){

             wmi2.clear();
            cpu.clear();
            hdd.clear();
            ip.clear();
             }else{
                 qDebug()<<qry.lastError().text();
             }
         }
            progress++;
            ui->progressBar->setValue(progress);


        }
        reselect();
            ui->progressBar->setVisible(false);
        QMessageBox msgBox;
          msgBox.setText("Данные загруженны.");
          msgBox.exec();

}
