#ifndef NAVI_H
#define NAVI_H
#include "mainwindow.h"
#include <QDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QAction>
#include <QWidget>
#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class Navi;
class MainWindow;
}

class Navi : public QDialog
{
    Q_OBJECT

public:
    explicit Navi(QMdiArea *md, QWidget *parent = nullptr);

    ~Navi();
 QMdiArea *mdiArea;

private slots:

    void keyPressEvent(QKeyEvent *e);

 void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

 void repaintBackup();

private:
    Ui::Navi *ui;
    QAction *action;
    QMdiSubWindow *subOrg;
    QThread thread_1;

    QTreeWidget *wid;
     QTreeWidgetItem *item3; //фл
     QTreeWidgetItem *item4;  // подразделения
     QTreeWidgetItem *item6;  //должности
     QTreeWidgetItem *item7; //структура организации
     QTreeWidgetItem *item8; //штатное расписание
     QTreeWidgetItem *item9; //список работников
     QTreeWidgetItem *item11; //список имениников
     QTreeWidgetItem *item12; // список свободных должностей
     QTreeWidgetItem *item13; // отчёт перепись общая
     QTreeWidgetItem *item15; //для бэкапов
      QTreeWidgetItem *item17; //для бэкапов
      QTreeWidgetItem *item18; //для колонок по кабинетам
     QTreeWidgetItem *item19; //для запчастей принтеров
     QTreeWidgetItem *item20; //для списка принтеров
     QTreeWidgetItem *item21; //для фир
     QTreeWidgetItem *item23; //для пользователи фир
     QTreeWidgetItem *item24; // отчёт по ринтерам
      QTreeWidgetItem *item25; // справочник картриджей
     QMdiSubWindow *subWindowB;


};

#endif // NAVI_H
