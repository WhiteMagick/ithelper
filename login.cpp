#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <QAction>
#include <QSqlError>
#include <QSettings>
#include <iostream>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <string>
#include <QSqlQuery>
#include<QDirIterator>
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle(tr("ПК IT HELPER"));
    msg = new QMessageBox;

    loadSetting();
}

Login::~Login()
{
    delete ui;
}



void Login::on_LoginButton_clicked()
{
    MainWindow *m= new MainWindow;
    db=QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={SQL Server};SERVER="+ui->serverName->text()+";DATABASE="+ui->dbName->text()+";Trusted_Connection=yes;");
    if(db.open()){
        this->close();
        m->showMaximized();
        saveSetting();
    }else{
        QMessageBox::critical(this,"Error","Соединение не установлено!"+db.lastError().text());
    }


}

void Login::saveSetting()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    // save value from QLineEdit
    QString text = ui->serverName->text();
    settings.setValue("Server", text);
     settings.setValue("db", ui->dbName->text());
}

void Login::loadSetting()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    // restore value of QLineEdit
    QString text = settings.value("Server","").toString();
    ui->serverName->setText(text);
    ui->dbName->setText(settings.value("db","").toString());

}

void Login::updateCheck(const QDir& dir,const QDir& dir2)
{
    //QDir dir("D:\\test1");  //объявляем объект работы с папками
QApplication::processEvents();

        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Hidden);     //получаем список файлов директории
        QFileInfoList list2 = dir2.entryInfoList(QDir::Files | QDir::Hidden);
        QList<QFileInfo> set =list;
        QList <QFileInfo> set1=list2;
       //QSet<QFileInfo> setL=QSet<QFileInfo>::fromList(set);


    QList<QFileInfo>::iterator i;
    for (i = set.begin(); i != set.end(); ++i){
        qDebug()<<i->fileName();
    qDebug()<<i->lastModified();}

}
