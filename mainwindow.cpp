#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "asistant.h"
#include "navi.h"
#include "comp.h"
#include "monitor.h"
#include "os.h"
#include "otdel.h"
#include "perepis.h"
#include "addcomp.h"
#include "addotdel.h"
#include "addmon.h"
#include "addperepis.h"
#include "search.h"
#include "printer.h"
#include "addprn.h"
#include "potoky.h"
#include "backup.h"
#include "otherhardware.h"
#include "zapchasti.h"
#include "salaryprn.h"
#include "kartridj.h"
#include <QSettings>
#include <QInputDialog>
#include <QSqlError>
#include <QTimerEvent>

MainWindow::MainWindow()
   //: mdiArea(new QMdiArea)
{

//    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea = new mdiarea(this);
    setCentralWidget(mdiArea);
    createAction();
    createMenu();
    createToolBar();
    setWindowTitle(tr("ПК IT HELPER v 3.5"));
    setWindowIcon(QPixmap(":/servicesmodificata.png"));
    setUnifiedTitleAndToolBarOnMac(true);
    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()), this, SLOT(createNavigator()));
    timer->start(1000);
    //createNavigator();
    mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation, true);

}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
  if(e->key()==Qt::Key_Escape){


  }
}



void MainWindow::createPerepis()
{
    Login *k = new Login;

 perepis *perep = new perepis(k->db,mdiArea);
 QMdiSubWindow *subWindow = mdiArea->addSubWindow(perep);
 QScreen *screen = qApp->primaryScreen();
 int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
 int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
 subWindow->setGeometry(x, y, 1300, 600);
 subWindow->show();
}

void MainWindow::delJurnal()
{
   QSqlQuery delJurnal;
   delJurnal.prepare("delete from JURNAL;");
   if(!delJurnal.exec()){
    QMessageBox::warning(this,"Ошибка",delJurnal.lastError().text());
}
   QMessageBox::information(this,"Сообщение","Журнал блокировок очищен.");
   }
void MainWindow::asist()
{
    qDebug()<<"ska";
 Asistant *asis = new Asistant(mdiArea);
 QMdiSubWindow *subWindow = mdiArea->addSubWindow(asis);
 QScreen *screen = qApp->primaryScreen();
 int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
 int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
 subWindow->setGeometry(x, y, 570, 270);
 subWindow->show();
}

void MainWindow::createAction()
{

    newAct = new QAction(tr("&Navigator"), this);
    newAct->setIcon(QIcon(":/nav.png"));
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Navigator"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(createNavigator()));
    ActPerepis = new QAction(tr("&Perepis"),this);
    ActPerepis->setIcon(QIcon(":/rep.png"));
    ActPerepis->setShortcut(QKeySequence::Paste);
    ActPerepis->setStatusTip(tr("Perepis"));
    connect(ActPerepis,SIGNAL(triggered()),this,SLOT(createPerepis()));
    cleanJurnal = new QAction(tr("&Clean Jurnal"),this);
    cleanJurnal->setIcon(QIcon(":/log-format.png"));
    cleanJurnal->setStatusTip(tr("Claen Jurnal"));
    connect(cleanJurnal,SIGNAL(triggered()),this,SLOT(delJurnal()));
//    dataColl = new QAction(tr("&Data Collection"));
//    dataColl = new QAction(tr("&Data Collection"),this);
//    dataColl->setIcon(QIcon(":/data.png"));
//    dataColl->setStatusTip(tr("Data Collection"));
//    connect(dataColl,SIGNAL(triggered()),this,SLOT(dataCollection()));

        closeAction = new QAction(tr("Cl&ose"), this);
        closeAction->setShortcut(QKeySequence::Close);
        closeAction->setStatusTip(tr("Close the active window"));
        connect(closeAction, SIGNAL(triggered()),
                mdiArea, SLOT(closeActiveSubWindow()));

        closeAllAction = new QAction(tr("Close &All"), this);
        closeAllAction->setStatusTip(tr("Close all the windows"));
        connect(closeAllAction, SIGNAL(triggered()), this, SLOT(close()));

        tileAction = new QAction(tr("&Tile"), this);
        tileAction->setStatusTip(tr("Tile the windows"));
        connect(tileAction, SIGNAL(triggered()),
                mdiArea, SLOT(tileSubWindows()));

        cascadeAction = new QAction(tr("&Cascade"), this);
        cascadeAction->setStatusTip(tr("Cascade the windows"));
        connect(cascadeAction, SIGNAL(triggered()),
                mdiArea, SLOT(cascadeSubWindows()));

        nextAction = new QAction(tr("Ne&xt"), this);
        nextAction->setShortcut(QKeySequence::NextChild);
        nextAction->setStatusTip(tr("Move the focus to the next window"));
        connect(nextAction, SIGNAL(triggered()),
                mdiArea, SLOT(activateNextSubWindow()));

        previousAction = new QAction(tr("Pre&vious"), this);
        previousAction->setShortcut(QKeySequence::PreviousChild);
        previousAction->setStatusTip(tr("Move the focus to the previous "
                                        "window"));
        connect(previousAction, SIGNAL(triggered()),
                mdiArea, SLOT(activatePreviousSubWindow()));

        backupchik = new QAction(tr("&Создать копию БД"), this);
        backupchik->setStatusTip(tr("Создать копию БД"));
        connect(backupchik, SIGNAL(triggered()),
                this,SLOT(backup()));

        Asist = new QAction(tr("Спаравка"),this);
        Asist->setShortcuts(QKeySequence::HelpContents);
        connect(Asist,SIGNAL(triggered()),this,SLOT(asist()));

}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
        windowMenu->addAction(closeAction);
        windowMenu->addAction(closeAllAction);
        windowMenu->addSeparator();
        windowMenu->addAction(tileAction);
        windowMenu->addAction(cascadeAction);
        windowMenu->addSeparator();
        windowMenu->addAction(nextAction);
        windowMenu->addAction(previousAction);
//        windowMenu->addAction(separatorAction);



            rezerv = menuBar()->addMenu(tr("&Backup"));
            rezerv->addAction(backupchik);


        asistant = menuBar()->addMenu(tr("&Справка"));
        asistant->addAction(Asist);

}

void MainWindow::createToolBar()
{
fileToolBar = addToolBar(tr("Navigator"));
fileToolBar->addAction(newAct);

fileToolBar->addAction(ActPerepis);
fileToolBar->addAction(cleanJurnal);
//fileToolBar->addAction(dataColl);
}

void MainWindow::createStatusBar1()
{

}

void MainWindow::backup()
{
    QSqlQuery q;
    bool ok;
    QSettings settings("settings.ini", QSettings::IniFormat);
    QString text = settings.value("bak","").toString();
    QString kol = QInputDialog::getText(this,tr("Введите путь"),tr("Сетевой + укажите в конце пути имя файла пример - \\IT.bak"),QLineEdit::Normal,text,&ok);
    if(ok){
        QSettings settings("settings.ini", QSettings::IniFormat);
        settings.setValue("bak", kol);

    q.prepare("exec backupchik :dir;");
    q.bindValue(":dir",kol);
    if(q.exec()){
    }else{
    qDebug()<<q.lastError();
        }
    }
}

void MainWindow::dataCollection()  //не успользуется отказ от ДКС
{


    QSqlQuery out;
    out.prepare("Select f.FIO, e.DATE_OUT from [i2722-app016].dks.dbo.FACES_MAIN_TBL f "
                "inner join [i2722-app016].dks.dbo.EMPLOYERS_TBL e on f.LINK = e.FACE_LINK "
                "where e.DATE_OUT >= GETDATE();");
    if(!(out.exec())){
        QMessageBox::information(this,"ERROR",out.lastError().text());
    }else{
    QString s,n;
    n="Увольняется:";
    while(out.next()){
       s="\n"+out.value(0).toString()+"  дата увольнеиня "+out.value(1).toDate().toString("dd.MM.yyyy")+"\n";
       n+=s;
    }
    QMessageBox::information(this,"Контроль данных",n);
    }

    QSqlQuery birth;
    birth.prepare("Select f.FIO, f.BIRTHDAY from [i2722-app016].dks.dbo.FACES_MAIN_TBL f "
                "inner join [i2722-app016].dks.dbo.EMPLOYERS_TBL e on f.LINK = e.FACE_LINK "
                "where MONTH(f.BIRTHDAY)=MONTH(GETDATE()) and (DAY(f.BIRTHDAY) BETWEEN DAY(GETDATE()) and DAY(GETDATE())+3) and e.DATE_OUT IS NULL;");
    birth.exec();
  QString k,v;
  v="День роджения:";
    while(birth.next()){

      k="\n"+birth.value(0).toString()+" "+birth.value(1).toDate().toString("dd.MM.yyyy")+"\n";
        v+=k;
    }


  QMessageBox::information(this,"Контроль данных",v);

//  QProcess *ping = new QProcess(0);
//  QString exec = "python";
//  QStringList param;
//  param<<"C:\\progi\\pyt\\test.py"<<n+v;
//  ping->start(exec,param);
//  ping->waitForFinished();
//  delete ping;

}

void MainWindow::createNavigator()
{
    if(timer->isActive()){
        timer->stop();
    }
    Navi *nav = new Navi(mdiArea);
    QMdiSubWindow *subWindow2 = mdiArea->addSubWindow(nav);
    subWindow2->setGeometry(0, 0, 428, 900);
    subWindow2->show();

}
