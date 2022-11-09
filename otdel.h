#ifndef OTDEL_H
#define OTDEL_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
namespace Ui {
class Otdel;
}

class Otdel : public QDialog
{
    Q_OBJECT

public:
    explicit Otdel(QMdiArea *md,QWidget *parent = nullptr);
    ~Otdel();
      int id;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void reselect();

    void keyPressEvent(QKeyEvent *e);
    void wordRep();
    void odbcExcel();

private:
    Ui::Otdel *ui;
    QSqlTableModel *model;
    QMdiArea *MdiAreaOtdel;
};

#endif // OTDEL_H
