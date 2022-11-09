#include "fir.h"
#include "ui_fir.h"
#include <QFileDialog>
#include <QAxObject>
#include <QMessageBox>
#include <QPixmap>
#include <QByteArray>
#include <QKeyEvent>
Fir::Fir(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fir)
{
    md= MdiAreaFir;
    ui->setupUi(this);
    setWindowTitle("Журнал запросов в ФИР");



}

Fir::~Fir()
{
    delete ui;
}

void Fir::loadUsers()
{


//    QMessageBox msgBox;
//      msgBox.setText("Идет обновление таблиы пользователей.");
//      msgBox.exec();
//   // QMessageBox::information(this,"Внимание","Идет обновление таблиы пользователей");
//    QSqlQuery del;
//    del.prepare("DELETE FROM UserDKS;");
//    del.exec();
//    QSqlQuery load;
//    load.prepare("insert into UserDKS (date_begin, FIO, Subdiv, Doljn) SELECT  MAX(i.DATE_BEGIN) AS 'DATE_BEGIN', f.FIO ,i.NEW_SUBDIV, i.NEW_POST FROM [i2722-app015].test.dbo.ITEM_MOVE i "
//                 "inner JOIN (select * from [i2722-app015].test.dbo.EMPLOYERS_TBL e where e.DATE_OUT IS NULL) e on i.FACE_LINK=e.FACE_LINK "
//                 "inner JOIN [i2722-app015].test.dbo.STAFF s on s.LINK = i.STAFF_LINK "
//                 "inner join [i2722-app015].test.dbo.FACES_MAIN_TBL f on f.LINK = i.FACE_LINK "
//                 "WHERE i.DATE_END = '9999-12-31 00:00:00.000' AND e.DATE_OUT is NULL AND s.DATE_END = '9999-12-31 00:00:00.000' "
//                 "group by i.FACE_LINK,f.FIO, i.NEW_SUBDIV,i.NEW_POST,i.DATE_END,e.DATE_OUT "
//                 "order by f.FIO;");
//    if(load.exec()){
//        QMessageBox::information(this,"Внимание","Список пользователей обновлен");
//    }else{
//        QMessageBox::warning(this,"Error", load.lastError().text());
//    }

}

void Fir::on_selectFile_clicked()
{

    QMessageBox *msgBox1 = new QMessageBox;
    QFileDialog dial(this);
    //dial.getOpenFileName(this,tr("Выбирите файл"),"C:\\",tr("Excel files(*.xls,*.xlsx)"));
    QString s= dial.getOpenFileName(this,tr("Выбирите файл"),"C:\\","Any fiels(*)");
    ui->label_2->setText(s);
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","xlsdb");
           db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};FIL={MS Excel};READONLY=FALSE;DBQ="+s+";");
    QSqlQuery qry;
           if(!db.open()){

               qDebug() << "Cannot open database:" << db.lastError();
           msgBox1->setText(db.lastError().text());
           msgBox1->exec();
           }
           QSqlQuery query(db);
           db.transaction();
           query.exec("SELECT * FROM [found$];");
            qDebug() <<  query.QSqlQuery::lastError();

           while (query.next()) {
               //QString name = query.value(0).toString();
               qDebug() << query.value(0).toString()+","+query.value(1).toString()+","+query.value(2).toString()+","+query.value(3).toString()+","+query.value(9).toString();
               qry.prepare("INSERT INTO JurnalFir (Login, FIO, Date_zp, systema, qry) VALUES ( :Logi, :FIO, :Data_zp, :systema, :qry);");
               qry.bindValue(":Logi",query.value(0).toString());
               qry.bindValue(":FIO",query.value(1).toString());
               qry.bindValue(":Data_zp",query.value(5).toString());
               qry.bindValue(":systema",query.value(6).toString());
               qry.bindValue(":qry",query.value(9).toString());
               qry.exec();

             }
           db.QSqlDatabase::commit();
           QMessageBox msgBox;
             msgBox.setText("Данные загруженны.");
             msgBox.exec();


}

void Fir::on_createJurnal_clicked()
{


QSqlQuery user;
user.prepare("select FIO, login from UserFir where st = 1;");
user.exec();
while (user.next()) {



    QString tmp_s=  QApplication::applicationDirPath();
        // получаем указатель на Excel
            QAxObject *mExcel = new QAxObject("Excel.Application",this);
            //видимость
            mExcel->dynamicCall( "SetVisible(bool)", false );
            // на книги
            QAxObject *workbooks = mExcel->querySubObject("Workbooks");
            // на директорию, откуда грузить книг
            QAxObject *workbook = workbooks->querySubObject( "Open(const QString&)",tmp_s+ "/FIR_qry.xltx" );
            // на листы
            QAxObject *mSheets = workbook->querySubObject( "Sheets" );
            // указываем, какой лист выбрать
            QAxObject *StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant("list") );

            QAxObject* fio = StatSheet->querySubObject("Cells(QVariant,QVariant)", 2, 1);
            fio->setProperty("Value", user.value(0).toString());

                int i,j,n;
                i=5;
                j=0;
                 n=1;


                    QSqlQuery parentsQuery;
                    parentsQuery.prepare("select j.systema,j.qry, j.Date_zp from JurnalFir j "
                                         "inner join UserFir u on j.Login like u.login "
                                         "where j.Login like :login "
                                         "order by j.Date_zp;");
                    parentsQuery.bindValue(":login",user.value(1).toString());

                    if(!parentsQuery.exec())
                        return;
                    while(parentsQuery.next())
                        {


                        j=1;
                        while(j<=6){
                           QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", i, j);
                           switch (j) {
                           case 1:{
                               cell->setProperty("Value", n);
                               // Указатель на ячейку
                               QString s = "A"+QString::number(i);
                               QAxObject *rangec = StatSheet->querySubObject( "Range(const QVariant&)",QVariant(s));
                               QAxObject *border = rangec->querySubObject("Borders(xlEdgeTop)");
                               border->setProperty("LineStyle",1);
                               border->setProperty("Weight",2);
                               QAxObject *border1 = rangec->querySubObject("Borders(xlEdgeRight)");
                               border1->setProperty("LineStyle",1);
                               border1->setProperty("Weight",2);
                               QAxObject *border2 = rangec->querySubObject("Borders(xlEdgeLeft)");
                               border2->setProperty("LineStyle",1);
                               border2->setProperty("Weight",2);
                               QAxObject *border3 = rangec->querySubObject("Borders(xlEdgeBottom)");
                               border3->setProperty("LineStyle",1);
                               border3->setProperty("Weight",2);
                               delete border;
                               delete border1;
                               delete border2;
                               delete border3;
                               delete rangec;

                               n++;
                               break; }
                           case 2:{

                               QString s = "B"+QString::number(i);
                               QAxObject *rangec = StatSheet->querySubObject( "Range(const QVariant&)",QVariant(s));
                               QAxObject *border = rangec->querySubObject("Borders(xlEdgeTop)");
                               border->setProperty("LineStyle",1);
                               border->setProperty("Weight",2);
                               QAxObject *border1 = rangec->querySubObject("Borders(xlEdgeRight)");
                               border1->setProperty("LineStyle",1);
                               border1->setProperty("Weight",2);
                               QAxObject *border2 = rangec->querySubObject("Borders(xlEdgeLeft)");
                               border2->setProperty("LineStyle",1);
                               border2->setProperty("Weight",2);
                               QAxObject *border3 = rangec->querySubObject("Borders(xlEdgeBottom)");
                               border3->setProperty("LineStyle",1);
                               border3->setProperty("Weight",2);
                               QVariant f=parentsQuery.value(0).toString();
                               qDebug()<<parentsQuery.value(0).toString();
                               cell->setProperty("Value", f);
                               delete border;
                               delete border1;
                               delete border2;
                               delete border3;
                               delete rangec;
                               break; }
                           case 3:{
                               QString s = "C"+QString::number(i);
                               QAxObject *rangec = StatSheet->querySubObject( "Range(const QVariant&)",QVariant(s));
                               QAxObject *border = rangec->querySubObject("Borders(xlEdgeTop)");
                               border->setProperty("LineStyle",1);
                               border->setProperty("Weight",2);
                               QAxObject *border1 = rangec->querySubObject("Borders(xlEdgeRight)");
                               border1->setProperty("LineStyle",1);
                               border1->setProperty("Weight",2);
                               QAxObject *border2 = rangec->querySubObject("Borders(xlEdgeLeft)");
                               border2->setProperty("LineStyle",1);
                               border2->setProperty("Weight",2);
                               QAxObject *border3 = rangec->querySubObject("Borders(xlEdgeBottom)");
                               border3->setProperty("LineStyle",1);
                               border3->setProperty("Weight",2);
                               QVariant f=parentsQuery.value(1).toString();
                                 qDebug()<<parentsQuery.value(1).toString();
                               cell->setProperty("Value", f);
                               delete border;
                               delete border1;
                               delete border2;
                               delete border3;
                               delete rangec;
                               break; }
                           case 4:{
                               QString s = "D"+QString::number(i);
                               QAxObject *rangec = StatSheet->querySubObject( "Range(const QVariant&)",QVariant(s));
                               QAxObject *border = rangec->querySubObject("Borders(xlEdgeTop)");
                               border->setProperty("LineStyle",1);
                               border->setProperty("Weight",2);
                               QAxObject *border1 = rangec->querySubObject("Borders(xlEdgeRight)");
                               border1->setProperty("LineStyle",1);
                               border1->setProperty("Weight",2);
                               QAxObject *border2 = rangec->querySubObject("Borders(xlEdgeLeft)");
                               border2->setProperty("LineStyle",1);
                               border2->setProperty("Weight",2);
                               QAxObject *border3 = rangec->querySubObject("Borders(xlEdgeBottom)");
                               border3->setProperty("LineStyle",1);
                               border3->setProperty("Weight",2);
                               QVariant f=parentsQuery.value(2).toString();
                                 qDebug()<<parentsQuery.value(2).toString();
                               cell->setProperty("Value", f);
                               delete border;
                               delete border1;
                               delete border2;
                               delete border3;
                               delete rangec;
                               break; }
                           case 5:{
                               QString s = "E"+QString::number(i);
                               QAxObject *rangec = StatSheet->querySubObject( "Range(const QVariant&)",QVariant(s));
                               QAxObject *border = rangec->querySubObject("Borders(xlEdgeTop)");
                               border->setProperty("LineStyle",1);
                               border->setProperty("Weight",2);
                               QAxObject *border1 = rangec->querySubObject("Borders(xlEdgeRight)");
                               border1->setProperty("LineStyle",1);
                               border1->setProperty("Weight",2);
                               QAxObject *border2 = rangec->querySubObject("Borders(xlEdgeLeft)");
                               border2->setProperty("LineStyle",1);
                               border2->setProperty("Weight",2);
                               QAxObject *border3 = rangec->querySubObject("Borders(xlEdgeBottom)");
                               border3->setProperty("LineStyle",1);
                               border3->setProperty("Weight",2);
                               cell->setProperty("Value", " ");
                               delete border;
                               delete border1;
                               delete border2;
                               delete border3;
                               delete rangec;
                               break; }

                           case 6:{
                               QString s = "F"+QString::number(i);
                               QAxObject *rangec = StatSheet->querySubObject( "Range(const QVariant&)",QVariant(s));
                               QAxObject *border = rangec->querySubObject("Borders(xlEdgeTop)");
                               border->setProperty("LineStyle",1);
                               border->setProperty("Weight",2);
                               QAxObject *border1 = rangec->querySubObject("Borders(xlEdgeRight)");
                               border1->setProperty("LineStyle",1);
                               border1->setProperty("Weight",2);
                               QAxObject *border2 = rangec->querySubObject("Borders(xlEdgeLeft)");
                               border2->setProperty("LineStyle",1);
                               border2->setProperty("Weight",2);
                               QAxObject *border3 = rangec->querySubObject("Borders(xlEdgeBottom)");
                               border3->setProperty("LineStyle",1);
                               border3->setProperty("Weight",2);
                               cell->setProperty("Value", "Информация не выгружалась");
                                delete border;
                                delete border1;
                                delete border2;
                                delete border3;
                                delete rangec;

                               break; }

                           }
                           j++;


                           delete cell;
                        }
                           i++;
                    QAxObject* cell1 = StatSheet->querySubObject("Cells(QVariant,QVariant)", i+1, 3);
                    cell1->setProperty("Value", " __________________");
                    QAxObject* cell2 = StatSheet->querySubObject("Cells(QVariant,QVariant)", i+2, 3);
                    cell2->setProperty("Value", " Наименование должности руководителя");

                    QAxObject* cell3 = StatSheet->querySubObject("Cells(QVariant,QVariant)", i+1, 5);
                    cell3->setProperty("Value", " __________________");
                    QAxObject* cell4 = StatSheet->querySubObject("Cells(QVariant,QVariant)", i+2, 5);
                    cell4->setProperty("Value", " ФИО");

                    QAxObject* cell5 = StatSheet->querySubObject("Cells(QVariant,QVariant)", i+1, 6);
                    cell5->setProperty("Value", " __________________");
                    QAxObject* cell6 = StatSheet->querySubObject("Cells(QVariant,QVariant)", i+2, 6);
                    cell6->setProperty("Value", "Подпись");

                    delete cell6;
                    delete cell5;
                    delete cell4;
                    delete cell3;
                    delete cell2;
                    delete cell1;






}
                    workbook->dynamicCall("SaveAS (const QString&)",QString("C:\\fir\\jurnal_"+user.value(0).toString())+".xlsx");
                    workbook->dynamicCall("Close (Boolean)",false);
                    //mExcel->setProperty("DisplayAlerts", 1);
                    mExcel->dynamicCall("Quit(void)");



                    delete fio;
                    delete StatSheet;
                    delete mSheets;
                    delete workbook;
                    delete workbooks;
                    delete mExcel;
}
            QMessageBox msgBox;
              msgBox.setText("Журналы сформированны.");
              msgBox.exec();


}


void Fir::on_cleanTbl_clicked()
{
    QSqlQuery clen;
    clen.prepare("DELETE FROM JurnalFir;");
    if(!clen.exec()){
         QMessageBox::warning(this,"Error", clen.lastError().text());
    }
    QMessageBox msgBox;
      msgBox.setText("Таблицы очищены.");
      msgBox.exec();

}

void Fir::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}
