#ifndef ADDOTHERH_H
#define ADDOTHERH_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>

namespace Ui {
class addOtherH;
}

class addOtherH : public QDialog
{
    Q_OBJECT

public:
    explicit addOtherH(int id,QMdiArea *md,QWidget *parent = nullptr);
    ~addOtherH();

    int id1;
signals:
    void Reselect();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_out_clicked();
     void closeEvent(QCloseEvent *event);

     void keyPressEvent(QKeyEvent *e);

private:
    Ui::addOtherH *ui;
    QMdiArea *MdiArea;
    QSqlQuery qry;
};

#endif // ADDOTHERH_H
