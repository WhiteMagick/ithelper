#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QTextEdit>
#include <QDialog>
#include <QMdiSubWindow>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QMenuBar>
#include <QApplication>
#include <QToolBar>
#include <QStatusBar>
#include <QWidget>
#include <QObject>
#include <QCloseEvent>
#include <QTimer>
#include <QListWidget>
#include <QLabel>
#include "mdiarea.h"
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QToolBar;
class Editor;
QT_END_NAMESPACE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
public slots:
    void keyPressEvent(QKeyEvent *e);
private slots:
    void createNavigator();
    void createPerepis();
    void delJurnal();
      void asist();
    void createAction();
    void createMenu();
    void createToolBar();
    void createStatusBar1();
    void backup();
    void dataCollection();

    //void closeEvent(QCloseEvent *event);


private:
    mdiarea*mdiArea;
    //QMdiArea *mdiArea;
    QMenu *windowMenu;
     QMenu *fileMenu;
     QMenu *rezerv;
     QMenu *asistant;
      QToolBar *fileToolBar;
    QAction *newAct;
    QAction *dataColl;
    QAction *ActPerepis;
    QAction *cleanJurnal;
    QAction *saveAsAct;
    QAction *recentFileSeparator;
    QAction *recentFileSubMenuAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *closeAction;
       QAction *closeAllAction;
       QAction *tileAction;
       QAction *cascadeAction;
       QAction *nextAction;
       QAction *previousAction;
    QAction *windowMenuSeparatorAct;
    QAction *backupchik;
    QAction *Asist;
    QTimer *timer;

};

#endif // MAINWINDOW_H
