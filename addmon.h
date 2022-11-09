#ifndef ADDMON_H
#define ADDMON_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>

namespace Ui {
class addMon;
}

class addMon : public QDialog
{
    Q_OBJECT

public:
    explicit addMon(int id,QMdiArea *md,QWidget *parent = nullptr);
    ~addMon();
int id1;

signals:
    void Reselect();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::addMon *ui;
    QMdiArea *MdiArea;
    QSqlQuery qry;
};

#endif // ADDMON_H
