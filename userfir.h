#ifndef USERFIR_H
#define USERFIR_H

#include <QDialog>
#include <QMdiArea>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QKeyEvent>
#include <QSqlError>
#include <QDebug>
#include <QSortFilterProxyModel>

namespace Ui {
class UserFIR;
}

class UserFIR : public QDialog
{
    Q_OBJECT

public:
    explicit UserFIR(QMdiArea *md,QWidget *parent = nullptr);
    ~UserFIR();



private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
    void keyPressEvent(QKeyEvent *e);

    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_find_clicked();

    void customMenu(QPoint pos);

    void CreateJurnal();

    void on_comboBox_activated(int index);
    void Reselect();

private:
    Ui::UserFIR *ui;
    QSqlTableModel *model;
    QSqlQueryModel *qryModel;
    QMdiArea *firU;
    QSortFilterProxyModel *proxi;
   QSqlQuery qry;
};

#endif // USERFIR_H
