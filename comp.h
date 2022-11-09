#ifndef COMP_H
#define COMP_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>
#include "zapros.h"
namespace Ui {
class Comp;
}

class Comp : public QDialog
{
    Q_OBJECT

public:
    explicit Comp(QMdiArea *md,QWidget *parent = nullptr);
    ~Comp();
    int id;
    //int filter;
    QString s;
    QString str;
    QStringList wmi;
     QStringList wmi2;
    QSqlQuery qryUp;
    int progress;
    LONGLONG ozu;
    LONGLONG sizeHDD;
    float cpuz;
    QString hdd;
    QString cpu;
      QString ip;
    int PC;
public slots:
    void getQry(QSqlQuery qry);
    void select1();
     void reselect();
     void insertWmi();
     void insertWmi2();
private slots:
    void on_ButtonAdd_clicked();

    void on_ButtonDell_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *e);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_activated(int index);

    void on_pushButton_3_clicked();
    void copyfromBuff();
     void customMenu(QPoint pos);
     void wmiselect();
     void otvetWMI(QStringList slist,int fl);
     void message(int i);

     void on_wmiFill_clicked();

signals:
     void setID(QString str);
     void setID2(QString str);
     void setStr(QString str);
     void send(int i);


private:
    Ui::Comp *ui;
    QSqlTableModel *model;
    QMdiArea *MdiAreaComp;
    QSqlQuery copy;
    QSqlQuery search;
    QSqlQuery qry;
    QSqlQueryModel *qryModel;
    Zapros *zap1;
    QThread *thread_1;
    Zapros *zap2;
    QThread *thread_2;
    QString query;
    QSqlDatabase db;

};

#endif // COMP_H
