#ifndef OTHERHARDWARE_H
#define OTHERHARDWARE_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"

namespace Ui {
class OtherHardware;
}

class OtherHardware : public QDialog
{
    Q_OBJECT

public:
    explicit OtherHardware(QMdiArea *md,QWidget *parent = nullptr);
    ~OtherHardware();
      int id;

private slots:
    void on_Button_add_clicked();

    void on_Button_dell_clicked();

    void on_Button_copy_clicked();

    void on_Button_update_clicked();

    void on_pushButton_search_clicked();

    void on_comboBox_activated(int index);

    void reselect();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::OtherHardware *ui;
    QSqlTableModel *model;
    QMdiArea *MdiAreaOther;
    QSqlQuery search;
    QSqlQuery copy;
    QSqlQueryModel *qryModel;
    QSqlQuery qry;
};

#endif // OTHERHARDWARE_H
