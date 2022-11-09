#ifndef ADDKARTRIDJ_H
#define ADDKARTRIDJ_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QCheckBox>
#include <QList>
#include <QWidget>
#include "checkboxmodel.h"
#include <QCloseEvent>

namespace Ui {
class AddKartridj;
}

class AddKartridj : public QDialog
{
    Q_OBJECT

public:
    explicit AddKartridj(int id, QMdiArea *md,QWidget *parent = nullptr);
    ~AddKartridj();
    int id1;
private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_exit_clicked();
    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *e);

signals:
    void Reselect();

private:
    Ui::AddKartridj *ui;
    QMdiArea *MdiAreaAddKar;
    checkboxModel *model;
    QSqlQuery qry;
    QSqlQuery qry2;
    QString spisok;
};

#endif // ADDKARTRIDJ_H
