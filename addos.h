#ifndef ADDOS_H
#define ADDOS_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>
namespace Ui {
class AddOs;
}

class AddOs : public QDialog
{
    Q_OBJECT

public:
    explicit AddOs(int id,QMdiArea *md,QWidget *parent = nullptr);
    ~AddOs();
    int id1;
signals:
    void Reselect();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::AddOs *ui;
    QSqlQuery qry1;
    QMdiArea *MdiArea;
};

#endif // ADDOS_H
