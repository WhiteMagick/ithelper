#ifndef VIBOROTDELA_H
#define VIBOROTDELA_H

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
namespace Ui {
class ViborOtdela;
}

class ViborOtdela : public QDialog
{
    Q_OBJECT

public:
    explicit ViborOtdela(QMdiArea *md, int id,int who, QWidget *parent = nullptr);
    ~ViborOtdela();
    int ot;
    int where;
signals:
    void Reselect();
private slots:
    void on_pushButton_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::ViborOtdela *ui;
    QMdiArea *mdiAreaVibor;
    QSqlTableModel *model;
    QSqlQueryModel *qryModel;
    QString st;
    QSqlQuery qry;
};

#endif // VIBOROTDELA_H
