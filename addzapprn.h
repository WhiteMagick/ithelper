#ifndef ADDZAPPRN_H
#define ADDZAPPRN_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QCheckBox>
#include <QList>
#include <QWidget>
#include <QCloseEvent>
namespace Ui {
class addZapPrn;
}

class addZapPrn : public QDialog
{
    Q_OBJECT

public:
    explicit addZapPrn(int id, QMdiArea *md,QWidget *parent = nullptr);
     int id1;
    ~addZapPrn();

private slots:
     void createChek();
     void on_pushButton_2_clicked();

     void createlist();
     void closeEvent(QCloseEvent *event);

     void keyPressEvent(QKeyEvent *e);
     void on_pushButton_clicked();

signals:
    void Reselect();

private:
    Ui::addZapPrn *ui;
    QMdiArea *MdiAreaAddZap;
    QSqlQuery qry;
    QList<QCheckBox*> listCheck;
    QString spisok;

};

#endif // ADDZAPPRN_H
