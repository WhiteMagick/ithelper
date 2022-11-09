#include "navi.h"
#include "ui_navi.h"
#include "comp.h"
#include "monitor.h"
#include "os.h"
#include "otdel.h"
#include "perepis.h"
#include "printer.h"
#include "backup.h"
#include "otherhardware.h"
#include "kolonki.h"
#include "zapchasti.h"
#include "salaryprn.h"
#include "fir.h"
#include "userfir.h"
#include "report.h"
#include "kartridj.h"
#include <QDebug>
#include<QString>
#include <string.h>
#include <iostream>
#include<QDataStream>
#include <QMessageBox>
#include<QList>
#include <QAxObject>
#include <QCoreApplication>
#include <QScreen>


Navi::Navi(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Navi)
{
    mdiArea=md;
    ui->setupUi(this);
    setWindowTitle(tr("Navigator"));

    ui->treeWidget->clear();
    wid= new QTreeWidget;
      //QTreeWidget *navi = new QTreeWidget(this);
      QTreeWidgetItem *item = new QTreeWidgetItem();
      item->setText(0,"Навигатор");
     ui->treeWidget->setColumnCount(1);
     ui->treeWidget->headerItem()->setHidden(1);
     ui->treeWidget->addTopLevelItem(item);
     QTreeWidgetItem *item2 = new QTreeWidgetItem();
     item2->setText(0,"Перепись");
     item2->setIcon(0,QIcon(":/folder.png"));
     item->addChild(item2);
     item7 = new QTreeWidgetItem();
     item7->setText(0,"Перепись по отделам");
     item7->setIcon(0,QIcon(":/rep.png"));
     item2->addChild(item7);
     item12 = new QTreeWidgetItem();
     item12->setText(0,"Принтера по отделам");
     item12->setIcon(0,QIcon(":/rep.png"));
     item2->addChild(item12);
     item18 = new QTreeWidgetItem();
     item18->setText(0,"Колонки по кабинетам");
     item18->setIcon(0,QIcon(":/rep.png"));
     item2->addChild(item18);
     QTreeWidgetItem *item5 = new QTreeWidgetItem();
     item5->setText(0,"Справочники");
     item5->setIcon(0,QIcon(":/folder.png"));
     item->addChild(item5);
     item4 = new QTreeWidgetItem();
     item4->setText(0,"Список компьютеров");
     item4->setIcon(0,QIcon(":/list.png"));
     item5->addChild(item4);
      item6 = new QTreeWidgetItem();
      item6->setText(0,"Список мониторов");
      item6->setIcon(0,QIcon(":/list.png"));
      item5->addChild(item6);
      item11 = new QTreeWidgetItem();
      item11->setText(0,"Список ОС");
      item11->setIcon(0,QIcon(":/list.png"));
      item5->addChild(item11);
      item8 = new QTreeWidgetItem();
      item8->setText(0,"Список отделов");
      item8->setIcon(0,QIcon(":/list.png"));
      item5->addChild(item8);
      QTreeWidgetItem *item14 = new QTreeWidgetItem();
      item14->setText(0,"Отчёты");
      item14->setIcon(0,QIcon(":/folder.png"));
      item->addChild(item14);
      item13 = new QTreeWidgetItem();
      item13->setText(0,"Перепись по отделам");
      item13->setIcon(0,QIcon(":/rep.png"));
      item24 = new QTreeWidgetItem();
      item24->setText(0,"Отчёт по принтерам");
      item24->setIcon(0,QIcon(":/rep.png"));
      item14->addChild(item13);
      item14->addChild(item24);
        Login *k = new Login;
      QSqlQuery *root = new QSqlQuery(k->db);
      root->prepare("SELECT IS_MEMBER ('Prosmotr');");
      if(!root->exec()){
         qDebug()<<root->lastError().text();
      }
      root->next();
      if(root->value(0).toInt()!=1){
      QTreeWidgetItem *item16 = new QTreeWidgetItem();
      item16->setText(0,"BACKUP");
      item16->setIcon(0,QIcon(":/folder.png"));
      item->addChild(item16);
      item15 = new QTreeWidgetItem();
      item15->setText(0,"Информация по дискам");
      item15->setIcon(0,QIcon("://data.png"));
      item16->addChild(item15);
      QTreeWidgetItem *item22 = new QTreeWidgetItem();
      item22->setText(0,"Для ФИРа");
      item22->setIcon(0,QIcon(":/folder.png"));
      item->addChild(item22);
      item21 = new QTreeWidgetItem();
      item21->setText(0,"Журнал запросов в фир");
      item21->setIcon(0,QIcon(":/rep.png"));
      item22->addChild(item21);
      item23 = new QTreeWidgetItem();
      item23->setText(0,"Пользователи ФИР");
      item23->setIcon(0,QIcon(":/sub.png"));
      item22->addChild(item23);
      }
      item17 = new QTreeWidgetItem();
      item17->setText(0,"Прочее оборудование");
      item17->setIcon(0,QIcon(":/list.png"));
      item5->addChild(item17);
      item19 = new QTreeWidgetItem();
      item19->setText(0,"Запчасти для принтеров");
      item19->setIcon(0,QIcon(":/list.png"));
      item5->addChild(item19);
      item20 = new QTreeWidgetItem();
      item20->setText(0,"Справочник принтеров");
      item20->setIcon(0,QIcon(":/list.png"));
      item5->addChild(item20);
      item25 = new QTreeWidgetItem();
      item25->setText(0,"Картриджи (справочник и склад)");
      item25->setIcon(0,QIcon(":/list.png"));
      item5->addChild(item25);



}

Navi::~Navi()
{
    delete ui;
}

void Navi::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}


void Navi::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item==item4)              //список компьютеров
        {
         Comp *comp = new Comp(mdiArea);
         QMdiSubWindow *subWindow = mdiArea->addSubWindow(comp);
         QScreen *screen = qApp->primaryScreen();
         int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
         int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
         subWindow->setGeometry(x, y, 1096, 400);
         subWindow->show();


    } else if(item==item6)              //список мониторов
    {
     Monitor *mon = new Monitor(mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(mon);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 752, 400);
     subWindow->show();
    }else if(item==item11)              //список операционых систем
    {
     OS *os = new OS(0,mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(os);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 300, 300);
     subWindow->show();
    }else if(item==item8)              //список отделов
    {
     Otdel *otdel = new Otdel(mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(otdel);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 400, 300);
     subWindow->show();
    }else if(item==item7)              //перепись по отделам
    {
        Login *k = new Login;

     perepis *perep = new perepis(k->db,mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(perep);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 1000, 600);
     subWindow->show();
    }
    else if (item==item12) {
        Login *k = new Login;

     Printer *prn = new Printer(k->db,mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(prn);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 1000, 600);
     subWindow->show();
    }
    else if (item == item13) {

        QString tmp_s=  QApplication::applicationDirPath();
            // получаем указатель на Excel
                QAxObject *mExcel = new QAxObject("Excel.Application",this);
                //видимость
                mExcel->dynamicCall( "SetVisible(bool)", true );
                // на книги
                QAxObject *workbooks = mExcel->querySubObject("Workbooks");
                // на директорию, откуда грузить книг
                QAxObject *workbook = workbooks->querySubObject( "Open(const QString&)",tmp_s+ "/perepis2.xltx" );
                // на листы
                QAxObject *mSheets = workbook->querySubObject( "Sheets" );
                // указываем, какой лист выбрать
                QAxObject *StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant("list") );
                int i,j;
                i=3;
                QSqlQuery parentsQuery;
                parentsQuery.prepare("select o.CODE,o.name, p.KAB, p.USERNAM, p.USERUZ, p.TEL, p.ROZTEL, p.ROZPC, c.name, CONCAT(c.maker,' ',c.model,' ') as 'Компьютер', "
                                     "c.inv,c.ser as 'серийный номер',c.hddZ,c.ramZ,CONCAT(m.maker,' ',m.model) as 'Монитор',m.inv,m.ser, c.eToken, p.COMMENT FROM perepis p "
                                     "inner join Otdel o on p.OTDELID=o.CODE "
                                     "inner join Comp c on p.COMPID=c.id "
                                     "inner join Monitor m on p.MONITORID = m.id "
                                     "order by CODE;");
                if(!parentsQuery.exec())
                    return;
                while(parentsQuery.next())
                    {
                    j=1;
                    while(j<=18){
                       QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", i, j);
                       switch (j) {
                       case 1:{
                           QVariant f=parentsQuery.value(1).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 2:{
                           QVariant f=parentsQuery.value(2).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 3:{
                           QVariant f=parentsQuery.value(3).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 4:{
                           QVariant f=parentsQuery.value(4).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 5:{
                           QVariant f=parentsQuery.value(5).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 6:{
                           QVariant f=parentsQuery.value(6).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 7:{
                           QVariant f=parentsQuery.value(7).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 8:{
                           QVariant f=parentsQuery.value(8).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 9:{
                           QVariant f=parentsQuery.value(9).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 10:{
                           QVariant f=parentsQuery.value(10).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 11:{
                           QVariant f=parentsQuery.value(11).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 12:{
                           QVariant f=parentsQuery.value(12).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 13:{
                           QVariant f=parentsQuery.value(13).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 14:{
                           QVariant f=parentsQuery.value(14).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 15:{
                           QVariant f=parentsQuery.value(15).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 16:{
                           QVariant f=parentsQuery.value(16).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 17:{
                           QVariant f=parentsQuery.value(17).toString();
                           cell->setProperty("Value", f);
                           break; }
                       case 18:{
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
    else if (item==item15) {
     backup *bak = new backup(mdiArea);
     subWindowB = mdiArea->addSubWindow(bak);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindowB->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindowB->height() / 2-150;
     subWindowB->setGeometry(x, y, 1000, 500);
     subWindowB->show();

    }
    else if (item==item17) {

      OtherHardware * oth = new OtherHardware(mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(oth);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 780, 350);
     subWindow->show();
    }

    else if (item==item18) {

    Kolonki *koln = new Kolonki(mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(koln);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 780, 350);
     subWindow->show();
}
    else if (item==item19) {

    Zapchasti *zap = new Zapchasti(0,mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(zap);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 580, 350);
     subWindow->show();
}
    else if (item==item20) {

   SalaryPrn *sprn = new SalaryPrn(mdiArea);
     QMdiSubWindow *subWindow = mdiArea->addSubWindow(sprn);
     QScreen *screen = qApp->primaryScreen();
     int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
     int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
     subWindow->setGeometry(x, y, 550, 350);
     subWindow->show();
}
    else if (item==item21) {
        Fir *fir = new Fir(mdiArea);
        QMdiSubWindow *subWindow = mdiArea->addSubWindow(fir);
        QScreen *screen = qApp->primaryScreen();
        int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
        int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
        subWindow->setGeometry(x, y, 420, 220);
        subWindow->show();

}
    else if (item==item23) {
        UserFIR *user = new UserFIR(mdiArea);
        QMdiSubWindow *subWindow = mdiArea->addSubWindow(user);
        QScreen *screen = qApp->primaryScreen();
        int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
        int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
        subWindow->setGeometry(x, y, 1000, 500);
        subWindow->show();

}
    else if (item==item24) {

    report *rep = new report();
    rep->repPrint();

    }
    else if (item==item25) {
        kartridj *kar = new kartridj(0,0,mdiArea);
        QMdiSubWindow *subWindow = mdiArea->addSubWindow(kar);
        QScreen *screen = qApp->primaryScreen();
        int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
        int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
        subWindow->setGeometry(x, y, 700, 500);
        subWindow->show();

}
}

void Navi::repaintBackup()
{

}
