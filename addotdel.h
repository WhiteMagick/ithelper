#ifndef ADDOTDEL_H
#define ADDOTDEL_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>
namespace Ui {
class AddOtdel;
}

class AddOtdel : public QDialog
{
    Q_OBJECT

public:
    explicit AddOtdel(int id,QMdiArea *md,QWidget *parent = nullptr);
    ~AddOtdel();
    int id1;
signals:
    void Reselect();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::AddOtdel *ui;
    QSqlQuery qry1;
    QMdiArea *MdiArea;

};

#endif // ADDOTDEL_H
