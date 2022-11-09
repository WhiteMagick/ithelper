#ifndef DEFHISTORY_H
#define DEFHISTORY_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QtWidgets>
#include <QDateEdit>
#include <QSortFilterProxyModel>

namespace Ui {
class defHistory;
}

class defHistory : public QDialog
{
    Q_OBJECT

public:
    explicit defHistory(QMdiArea *md,int id,int idP,QWidget *parent = nullptr);
    ~defHistory();

private slots:
    void createHistZap();
    void createHistPoZap();

    void keyPressEvent(QKeyEvent *e);

    void onClick_search();

private:
    Ui::defHistory *ui;
    QSqlTableModel *tmodel;
    QSqlQueryModel *qmodel;
    QMdiArea *MdiAreaHist;
    int id1;
    int idPrn;
    QDateEdit *dateF;
    QDateEdit *dateL;
    QVBoxLayout *vboxmain;
    QHBoxLayout *hboxUP;
    QTableView *view;
    QPushButton *search;
    QLabel *text;
    QLabel *text1;
    QLineEdit *searchLn;
    QSortFilterProxyModel *proxi;

};

#endif // DEFHISTORY_H
