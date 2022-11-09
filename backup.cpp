#include "backup.h"
#include "ui_backup.h"
#include "clickable.h"
#include <windows.h>
#include "navi.h"
#include <QFileSystemModel>
#include <QDir>
#include <QDirModel>
#include <QSettings>
#include <QFileDialog>
#include <QDesktopServices>
#include <QFile>
#include <QProgressBar>
#include <iostream>
#include<QDataStream>
#include <QMessageBox>
#include<QList>
#include <QAxObject>
#include <QCoreApplication>
#include <QKeyEvent>
backup::backup(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::backup)
{
       ui->setupUi(this);

MdiArea=md;

   createWindow();

}

backup::~backup()
{
    delete ui;

}

void backup::createWindow()
{
    hboxMain = new QHBoxLayout;
    vboxleft = new QVBoxLayout;
    vboxright = new QVBoxLayout;
    hboxBtnLow = new QHBoxLayout;
    hboxBtnHight = new QHBoxLayout;
    btnAdd = new QPushButton;
    close = new QPushButton;
    text = new QLabel;
    text1 = new QLabel;
    view = new QTableView;
    vboxleft->setSpacing(10);
    btnAdd->setIcon(QIcon("://plus.png"));
    btnAdd->setIconSize(QSize(25,25));
    text1->setText("Добавить диск");
    text->setText("Информация о дисках");


    hboxMain->setContentsMargins(10,10,10,10);
    hboxMain->setSpacing(20);
    hboxMain->addLayout(vboxleft);
    hboxMain->addLayout(vboxright);
    vboxleft->setContentsMargins(10,10,10,10);
    vboxleft->addLayout(hboxBtnHight);
    hboxBtnHight->addWidget(text1);
    hboxBtnHight->addWidget(btnAdd);
    vboxleft->addWidget(text);
    vboxright->addWidget(view);
    vboxright->addLayout(hboxBtnLow);
    hboxBtnLow->addWidget(close);
    setLayout(hboxMain);
   model = new QFileSystemModel;
   setStyleSheet("font-size: 14px;");
   connect(btnAdd,SIGNAL(clicked()),this,SLOT(addDisk()));
   connect(close,SIGNAL(clicked()),this,SLOT(test()));
   connect(view,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(moveTo(QModelIndex)));
//   Navi* n = new Navi(MdiArea);
//   connect(this,SIGNAL(repaint()),n,SLOT(repaintBackup()));
   restoreDisk();
}

void backup::addDisk()
{

    QFileDialog diag;
    diag.setFileMode(QFileDialog::Directory);
    diag.exec();
    QSettings settings("backup.ini", QSettings::IniFormat);
    settings.beginGroup("disk");
    settings.setValue(diag.directory().dirName(), diag.directory().path());
    settings.endGroup();

    while (QLayoutItem* item = vboxleft->takeAt(2)) {
        delete item->widget();
        delete item;
    }


    restoreDisk();



}

void backup::test()
{




}

void backup::restoreDisk()
{
    QSettings settings("backup.ini", QSettings::IniFormat);
    if(settings.allKeys().isEmpty()){
    }else{
    settings.beginGroup("disk");
    QStringList key = settings.allKeys();
    for(int i=0; i<key.size();i++){
    qDebug()<<settings.value(key.at(i)).toString();
    wgt = new QFrame();
    wgt->setFixedSize(300,105);
    wgt->setObjectName(QString::number(i));
    wgt->setFrameStyle(QFrame::Raised | QFrame::Sunken);
    wgt->setLineWidth(3);
    wgt->setStyleSheet("background-color: #FFFFFF; border: 1px solid #CCCCCC;\
                              border-radius: 4px; color: #555555;\
                              ");


    vboxleft->addWidget(wgt);
    QVBoxLayout* vert = new QVBoxLayout;
    QHBoxLayout* hbox = new QHBoxLayout;
    wgt->setLayout(vert);
    vert->addLayout(hbox);
    QLabel* disk = new Clickable();
    disk->setText(settings.value(key.at(i),"").toString());
    hbox->addWidget(disk);
    hbox->setSpacing(10);
    dell = new QPushButton;
    //dell->setText(QString::number(i));
      dell->setObjectName(QString::number(i));
    dell->setIcon(QIcon("://clear.png"));
    dell->setIconSize(QSize(25,25));
    dell->setFixedSize(37,33);
    dell->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    hbox->addWidget(dell);
    QProgressBar *size = new QProgressBar;
    double fTotal;
    double fFree;
    ULARGE_INTEGER free,total;
    bool bRes = ::GetDiskFreeSpaceExA( settings.value(key.at(i),"").toString().toLocal8Bit(), &free , &total , NULL );
    if ( !bRes ) qDebug() <<"lll";
               fFree = static_cast<__int64>(free.QuadPart);
                fTotal = static_cast<double>(static_cast<__int64>(total.QuadPart));


    size->setMaximum(static_cast<int>(fTotal/1024/1024/1024));
    size->setMinimum(static_cast<int>(fFree/1024/1024/1024));
    size->setValue(static_cast<int>(fTotal/1024/1024/1024)-static_cast<int>(fFree/1024/1024/1024));
    vert->addWidget(size);
    QLabel * lb = new QLabel;
    lb->setText(QString::number(static_cast<int>(fFree/1024/1024/1024))+"Гб свободно из "+QString::number(static_cast<int>(fTotal/1024/1024/1024))+"Гб");
    vert->addWidget(lb);
    connect(disk,SIGNAL(clicked(QString)),this,SLOT(labelClik(QString)));
    connect(dell,SIGNAL(clicked()),this,SLOT(dellDisk()));

     }
    settings.endGroup();
    }
    vboxleft->addStretch(1);
}

void backup::labelClik(QString st)
{


    Rootdir.setPath(st);
    model->setFilter(QDir::AllEntries);
        model->setRootPath(Rootdir.path());
        view->setModel(model);

        view->setRootIndex(model->index(Rootdir.path()));



}

void backup::moveTo(const QModelIndex &index)
{
   QFileSystemWatcher watch;

   QListView* listviw = (QListView*)sender();
   QFileInfo fileinfo= model->fileInfo(index);
   connect(&watch,SIGNAL(directoryChanged(const QString &)),this,SLOT(refresh(const QString &)));

   if(fileinfo.fileName()==".."){
       QDir dir = fileinfo.dir();
       dir.cdUp();
       listviw->setRootIndex( model->index(dir.absolutePath()));
       watch.addPath(dir.absolutePath());
   }
   else if(fileinfo.fileName()=="."){
      listviw->setRootIndex(model->index(Rootdir.path()));
       watch.addPath(Rootdir.path());
   }
   else if(fileinfo.isDir()){
       listviw->setRootIndex(index);
      watch.addPath(fileinfo.dir().absolutePath());
   }
}

void backup::dellDisk()
{
    QObject *obj = QObject::sender();
    qDebug() << obj->objectName();
    int i = obj->objectName().toInt();
    int k = dell->objectName().toInt();
    QSettings settings("backup.ini", QSettings::IniFormat);
    QStringList keys = settings.allKeys();
    settings.remove(keys.at(i));
    //qDebug()<< k;


    while (QLayoutItem* item = vboxleft->takeAt(2)) {
        delete item->widget();
        delete item;
    }


    restoreDisk();




}

void backup::refresh(const QString &d)
{
    qDebug()<<"refresh";
    QDir f;
    f.setPath(d);
    f.refresh();
}

void backup::slotButtonClicked()
{


}

void backup::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }

}



//void backup::closeEvent(QCloseEvent *event)
//{
//     event->accept();
//}
