#ifndef PRINTER_H
#define PRINTER_H

#include <QDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QAction>
#include <QWidget>
#include <QObject>
#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include "login.h"
#include <QSqlError>
#include <QDate>
#include <QApplication>
#include <QDir>

namespace Ui {
class Printer;
}

class Printer : public QDialog
{
    Q_OBJECT

public:
    explicit Printer(QSqlDatabase db,QMdiArea *md,QWidget *parent = nullptr);
    ~Printer();

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonDell_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void reselect();

    void on_pushButtonSearch_clicked();

    void on_comboBox_activated(int index);

    void on_pushButton_clicked();

    void customMenu(QPoint pos);

    void editSt();

    void editStActual();

    void on_Report_Button_clicked();

    void instZap();

    void historyZap();

    void instKart();

    void historyKart();

    void history();

    void keyPressEvent(QKeyEvent *e);

    void msgZap();

    void on_pushButton_hist_clicked();

private:
    Ui::Printer *ui;
    QMdiArea *mdiAreaPrinter;
    QSqlQueryModel *qryModel;
   QString st;
    QSqlQuery qry;
    QSqlQuery otId;
    QSqlQuery searchPrn;
    QString reportQry;
    QString search;
    int id;
    int ot;
    int status;

};

#endif // PRINTER_H
