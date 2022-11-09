#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include "comp.h"
#include "monitor.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"

namespace Ui {
class Search;
}

class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(QMdiArea *md,int id,QString g,QWidget *parent = nullptr);
    ~Search();
    int vib;
    QString GUID;
signals:
    void Vibor(int id);
private slots:
    void on_pushButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::Search *ui;
    QSqlTableModel *model;
    QSqlQueryModel *qmodel;
    QMdiArea *MdiAreaSearch;
    QSqlQuery pc;
    QSqlQuery mon;
    QSqlQuery kol;
    QSqlQuery per;
    int id1;
};

#endif // SEARCH_H
