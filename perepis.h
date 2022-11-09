#ifndef PEREPIS_H
#define PEREPIS_H

#include <QDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QAbstractItemView>
#include <QAction>
#include <QWidget>
#include <QObject>
#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include "login.h"
#include <QSqlError>
#include <QDate>
#include <QApplication>
#include <QDir>
namespace Ui {
class perepis;
class MainWindow;
}

class perepis : public QDialog
{
    Q_OBJECT

public:
    explicit perepis(QSqlDatabase db,QMdiArea *md,QWidget *parent = nullptr);
    ~perepis();


private slots:
    void on_ButtonAdd_clicked();

    void on_pushButtonDell_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_tableView_doubleClicked(const QModelIndex &index);
    void reselect();

    void on_pushButton_clicked();

    void ping();

   void openExpl();

    void customMenu(QPoint pos);

    void on_pushButton_2_clicked();

    void on_Report_Button_clicked();

    void editSt();

    void StTehSost();

    void History();

    //void on_tableView_clicked(const QModelIndex &index);

    void curentActiv(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *e);

    void on_comboBox_activated(int index);

    void on_HistoryButton_clicked();

    void report();



private:
    Ui::perepis *ui;
     QMdiArea *mdiArea;
     QMdiSubWindow *subwindowEmpl;
     QSqlTableModel *model;
     QSqlQueryModel *qryModel;
     QApplication *p;
    QString st;
      QSqlQuery qry1;
     QSqlQuery qry;
     QSqlQuery otId;
     QSqlQuery searchPC;
     QSqlQuery searchMon;
     QSqlQuery searchFIO;
     QString search;
     QString reportQry;
     int id;
     int id2;
     int ot;
     int status;
     QAction *select;
};

#endif // PEREPIS_H
