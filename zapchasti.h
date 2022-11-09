#ifndef ZAPCHASTI_H
#define ZAPCHASTI_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QKeyEvent>
#include <QMdiArea>
#include <QSortFilterProxyModel>

namespace Ui {
class Zapchasti;
}

class Zapchasti : public QDialog
{
    Q_OBJECT

public:
    explicit Zapchasti(int idP, QMdiArea *md, QWidget *parent = nullptr);
    ~Zapchasti();
    int id;
    int idPrn;
 signals:
    void Complite();

private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_hide_clicked();

    void on_pushButton_clicked();

    void history();

    void setStatus();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_refresh_clicked();

    void on_pushButton_hist_clicked();

    void customMenu(QPoint pos);

    void reselect();

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::Zapchasti *ui;
    QSqlTableModel *model;
    QMdiArea *MdiAreaZap;
    QSqlQuery search;
    QSqlQueryModel *qryModel;
    QSqlQuery qry;
    QSortFilterProxyModel *proxi;
    int click;
};

#endif // ZAPCHASTI_H
