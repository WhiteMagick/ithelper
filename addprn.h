#ifndef ADDPRN_H
#define ADDPRN_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>

namespace Ui {
class AddPrn;
}

class AddPrn : public QDialog
{
    Q_OBJECT

public:
    explicit AddPrn(int id, int ot, QMdiArea *md, QWidget *parent = nullptr);
    ~AddPrn();
 int id1;
 int otdel;
signals:
    void Reselect();
private slots:
    void fillcombo();
    void fillcomboforEdit();
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *e);

    void on_comboBox_3_currentIndexChanged(int index);

private:
    Ui::AddPrn *ui;
    QMdiArea *MdiAreaPrn;
    QSqlQuery qry;
};

#endif // ADDPRN_H
