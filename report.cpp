#include "report.h"
#include <QDebug>
#include<QString>
#include <string.h>
#include <iostream>
#include<QDataStream>
#include <QMessageBox>
#include<QList>
#include <QAxObject>
#include <QCoreApplication>

report::report(QObject *parent) : QObject(parent)
{

}

void report::repPrint()
{
   qDebug()<<"sssss";
    QString tmp_s= QApplication::applicationDirPath();
        // получаем указатель на Excel
            QAxObject *mExcel = new QAxObject("Excel.Application",this);
            //видимость
            mExcel->dynamicCall( "SetVisible(bool)", true );
            // на книги
            QAxObject *workbooks = mExcel->querySubObject("Workbooks");
            // на директорию, откуда грузить книг
            QAxObject *workbook = workbooks->querySubObject( "Open(const QString&)",tmp_s+ "/repPrn.xltx" );
            // на листы
            QAxObject *mSheets = workbook->querySubObject( "Sheets" );
            // указываем, какой лист выбрать
            QAxObject *StatSheet = mSheets->querySubObject( "Item(const QVariant&)", QVariant("list") );
            int i,j;
            i=3;
            QSqlQuery parentsQuery;
            parentsQuery.prepare("select concat(m.maker,' ',s.model),s.model,COUNT(p.model) as 'Кол-во' from Printer p "
                                 "inner join SalaryPrn s on s.id = p.model "
                                 "inner join SalaryMakerPRN m on s.maker=m.id "
                                 "Group BY s.model,m.maker;");
            if(!parentsQuery.exec())
                return;
            while(parentsQuery.next())
                {
                QSqlQuery * st2 = new QSqlQuery; //сломанные
                st2->prepare("select p.model, COUNT(p.STATUS) as 'кол-во' from Printer p "
                            "inner join SalaryPrn s on s.id = p.model "
                            "where s.model like :sl and p.STATUS = 1 "
                            "Group BY p.model;");
                st2->bindValue(":sl",parentsQuery.value(1).toString());
                st2->exec();
                st2->next();
                QSqlQuery * st1 = new QSqlQuery; //целые
                st1->prepare("select p.model, COUNT(p.STATUS) as 'кол-во' from Printer p "
                            "inner join SalaryPrn s on s.id = p.model "
                            "where s.model like :sl and p.STATUS != 1  "
                            "Group BY p.model;");
                st1->bindValue(":sl",parentsQuery.value(1).toString());
                st1->exec();
                st1->next();
                QSqlQuery * st3 = new QSqlQuery; //списанные
                st3->prepare("select p.model, COUNT(p.STATUS) as 'кол-во' from Printer p "
                            "inner join SalaryPrn s on s.id = p.model "
                            "where s.model like :sl and p.spisan = 1 "
                            "Group BY p.model;");
                st3->bindValue(":sl",parentsQuery.value(1).toString());
                st3->exec();
                st3->next();

                j=1;
                while(j<=5){
                   QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", i, j);
                   switch (j) {
                   case 1:{
                       QVariant f=parentsQuery.value(1).toString();
                       cell->setProperty("Value", f);
                       break; }
                   case 2:{
                       QVariant f=parentsQuery.value(2).toInt();
                       cell->setProperty("Value", f);
                       break; }
                   case 3:{
                       QVariant f=st1->value(1).toInt();
                       cell->setProperty("Value", f);
                       break; }
                   case 4:{
                       QVariant f=st2->value(1).toInt();
                       cell->setProperty("Value", f);
                       break; }
                   case 5:{
                       QVariant f=st3->value(1).toInt();
                       cell->setProperty("Value", f);
                       break; }

                       delete st1;
                       delete st2;
                       delete st3;


                   }
                   j++;
                   delete cell;
                }
                   i++;
                }



            mExcel->setProperty("DisplayAlerts", 1);
}
