#include "zapros.h"
#include <QSettings>
#include "login.h"
#include <io.h>
#include <stdfix.h>
#include <QTextStream>


Zapros::Zapros(QObject *parent)
    : QObject(parent),
     m_str(""),hdd(""),cpu(""),ip("")

{


}

QSqlQuery Zapros::qry() const
{
    return m_qry;
}

QString Zapros::str() const
{
    return m_str;
}

void Zapros::run()
{

}

void Zapros::select()
{
    int err=0; // для выхода из цикла запросов к wmi. После первого ошибочного.
     int c=0; //для прербора запросов к wmi
    ozu=0;
    sizeHDD=0;
    cpuz=0;
    bool ok;
      HRESULT hr = CoInitialize(0);
     // for (int c=0;c<=2;c++)
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
                      QString(namePC+".regions.tax.nalog.ru"),QString("root\\cimv2"));
          if(!objWMIService){
              qDebug("objWMIService not created");
              err=1;
              throw objWebmLocator->isNull();
              delete objWebmLocator;
          }//Win32_DiskDrive   Win32_Processor  Win32_IP4RouteTable

          QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))",query);
          if(!objInterList){
              qDebug("Answer from ExecQuery is null");
              delete objWebmLocator;
          }
          if(objInterList->dynamicCall("count").toInt()==0){
              qDebug("Count is 0");
              delete objWebmLocator;
          }

          QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
          if(!enum1){
              qDebug("Query _NewEnum failed");
              delete objWebmLocator;
          }
          IEnumVARIANT *enumInterface = 0;
          enum1->queryInterface(IID_IEnumVARIANT,(void**)&enumInterface);
          if(!enumInterface){
              qDebug("Query interface failed");
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

     qDebug()<<cpuz<<" cpuz" <<cpuz/1000;
     qDebug()<<ozu1<<"-"<<ozu2<<"-"<<ozu3;
     QStringList list2;
     list2 <<cpu <<hdd <<ip <<QString::number(ozu3) <<QString::number(sizeH) <<QString::number(cpuz/1000);
      if(list2.at(0)==""){
          qDebug()<<"пусто";
          qFill(list2.begin(),list2.end(),"-");
          cpu.clear();
          hdd.clear();
          ip.clear();
          emit sendWMI(list2,1);
      }else{
     qDebug()<<"не пусто";
     emit sendWMI(list2,1);
    list2.clear();
    cpu.clear();
    hdd.clear();
    ip.clear();
 }


}

void Zapros::finish()
{

}


void Zapros::setQry(QSqlQuery qry)
{

    m_qry = qry;
    emit qryChanged(m_qry);
}

void Zapros::setStr(QString str)
{

    m_str = str;
QSqlDatabase clon=QSqlDatabase::addDatabase("QODBC");
    QSettings settings("settings.ini", QSettings::IniFormat);
   clon.setDatabaseName("DRIVER={SQL Server};SERVER="+settings.value("Server","").toString()+";DATABASE="+settings.value("db","").toString()+";Trusted_Connection=yes;");
    if(!clon.open()){
        qDebug()<<"sosi";
    }
        else{
        qDebug()<<"в потоке";
        QSqlQuery zap;
       if(zap.exec(m_str)){
        //zap.next();
        emit send(zap);
        emit finished();
       }else {
        qDebug()<<zap.lastError().text();
       }
        }


}



void Zapros::selectWMI()
{
   int err=0; // для выхода из цикла запросов к wmi. После первого ошибочного.
    int c=0; //для прербора запросов к wmi
   ozu=0;
   sizeHDD=0;
   bool ok;
     HRESULT hr = CoInitialize(0);
    // for (int c=0;c<=2;c++)
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
                     QString(namePC+".regions.tax.nalog.ru"),QString("root\\cimv2"));
         if(!objWMIService){
             qDebug("objWMIService not created");
             err=1;
             throw objWebmLocator->isNull();
             delete objWebmLocator;
         }//Win32_DiskDrive   Win32_Processor  Win32_IP4RouteTable

         QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))",query);
         if(!objInterList){
             qDebug("Answer from ExecQuery is null");
             delete objWebmLocator;
         }
         if(objInterList->dynamicCall("count").toInt()==0){
             qDebug("Count is 0");
             delete objWebmLocator;
         }

         QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
         if(!enum1){
             qDebug("Query _NewEnum failed");
             delete objWebmLocator;
         }
         IEnumVARIANT *enumInterface = 0;
         enum1->queryInterface(IID_IEnumVARIANT,(void**)&enumInterface);
         if(!enumInterface){
             qDebug("Query interface failed");
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

    qDebug()<<cpuz<<" cpuz" <<cpuz/1000;
    qDebug()<<ozu1<<"-"<<ozu2<<"-"<<ozu3;
    QStringList list2;
    list2 <<cpu <<hdd <<ip <<QString::number(ozu3) <<QString::number(sizeH) <<QString::number(cpuz/1000);
     if(list2.at(0)==""){
         qDebug()<<"пусто";
         qFill(list2.begin(),list2.end(),"-");
         cpu.clear();
         hdd.clear();
         ip.clear();
         emit sendWMI(list2,0);
     }else{
    qDebug()<<"не пусто";
    emit sendWMI(list2,0);
   list2.clear();
   cpu.clear();
   hdd.clear();
   ip.clear();
}


}

void Zapros::setName(QString str)
{
    namePC = str;
    qDebug()<<namePC;
    selectWMI();

}

void Zapros::setName2(QString str)
{
    namePC = str;
    qDebug()<<namePC;
    select();
}


