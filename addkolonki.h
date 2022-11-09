#ifndef ADDKOLONKI_H
#define ADDKOLONKI_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>

namespace Ui {
class AddKolonki;
}

class AddKolonki : public QDialog
{
    Q_OBJECT

public:
    explicit AddKolonki(int id,QMdiArea *md,QWidget *parent = nullptr);
    ~AddKolonki();
      int id1;
signals:
    void Reselect();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
     void closeEvent(QCloseEvent *event);
     void keyPressEvent(QKeyEvent *e);

private:
    Ui::AddKolonki *ui;
    QMdiArea *MdiAreakol;
    QSqlQuery qry;
    int st;
};

#endif // ADDKOLONKI_H
