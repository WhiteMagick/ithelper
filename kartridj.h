#ifndef KARTRIDJ_H
#define KARTRIDJ_H

#include <QDialog>
#include <QMdiArea>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QKeyEvent>
#include <QSortFilterProxyModel>
#include <QTableView>

namespace Ui {
class kartridj;
}

class kartridj : public QDialog
{
    Q_OBJECT

public:
    explicit kartridj(int idP, int idPrnP, QMdiArea *md, QWidget *parent = nullptr);
    ~kartridj();
    int id;
    int idPrn;
    int prn;
    void setvale(int s);

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_upd_clicked();

    void reselect();

    void on_tableView_salary_doubleClicked(const QModelIndex &index);

    void on_tableView_sklad_doubleClicked(const QModelIndex &index);
    void doubleClickforVibor(const QModelIndex &index);

    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *e);
signals:
   void Complite();

private:
    Ui::kartridj *ui;

    QMdiArea *MdiAreakar;
    QSqlQuery search;
    QSqlQueryModel *qryModel;
    QSqlQuery qry;
    QSqlQuery qry2;
    QSortFilterProxyModel *proxi;
    QSqlTableModel *model2;
   QSortFilterProxyModel *proxi2;
    QTableView *vibor;

    int click;
};

#endif // KARTRIDJ_H
