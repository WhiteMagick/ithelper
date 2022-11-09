#ifndef ADDPEREPIS_H
#define ADDPEREPIS_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>

namespace Ui {
class addPerepis;
}

class addPerepis : public QDialog
{
    Q_OBJECT

public:
    explicit addPerepis(int id,int idOT,QMdiArea *md,QWidget *parent = nullptr);
    ~addPerepis();
int id1;
int viborPc;
int viborMon;
int idOtdel;

public slots:
     void viborPC(int id1);
     void viborMON(int id1);

signals:
    void Reselect();

private slots:
    void on_viborPC_clicked();

    void on_viborMon_clicked();

    void on_save_clicked();

    void on_exit_clicked();

    void on_viewPK_clicked();

    void on_viewMon_clicked();
    void fill();

    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::addPerepis *ui;
    QMdiArea *MdiAreaP;
    QSqlQuery qry;
    QSqlQuery qryP;
    QSqlQuery qryM;

    QList<QString> statusList;
    QList<QString> statusBH;

};

#endif // ADDPEREPIS_H
