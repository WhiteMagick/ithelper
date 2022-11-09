#ifndef MONITOR_H
#define MONITOR_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
namespace Ui {
class Monitor;
}

class Monitor : public QDialog
{
    Q_OBJECT

public:
    explicit Monitor(QMdiArea *md,QWidget *parent = nullptr);
    ~Monitor();
    int id;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);
     void reselect();

     void on_pushButton_3_clicked();

     void on_pushButton_4_clicked();
     void copyfromBuff();
      void customMenu(QPoint pos);
      void on_comboBox_activated(int index);

      void on_pushButton_5_clicked();

      void keyPressEvent(QKeyEvent *e);

private:
    Ui::Monitor *ui;
    QSqlTableModel *model;
    QMdiArea *MdiAreaMon;
    QSqlQuery search;
    QSqlQuery copy;
    QSqlQueryModel *qryModel;
    QSqlQuery qry;
};

#endif // MONITOR_H
