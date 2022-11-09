#ifndef ADDCOMP_H
#define ADDCOMP_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
#include <QMdiSubWindow>
namespace Ui {
class AddComp;
}

class AddComp : public QDialog
{
    Q_OBJECT

public:
    explicit AddComp(int id,QMdiArea *md, QWidget *parent = nullptr);
    ~AddComp();
    int id1;
public slots:
     void viborOS(int id1);
signals:
    void Reselect();
private slots:
    void on_pushButton_clicked();

    void on_btnSave_clicked();

    void on_pushButtonExit_clicked();

    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *e);


private:
    Ui::AddComp *ui;
    QMdiArea *MdiAreaComp;
    QSqlQuery qry;

};

#endif // ADDCOMP_H
