#ifndef ADDUSERFIR_H
#define ADDUSERFIR_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include <QKeyEvent>
namespace Ui {
class addUserFir;
}

class addUserFir : public QDialog
{
    Q_OBJECT

public:
    explicit addUserFir(int id,QMdiArea *md,QWidget *parent = nullptr);
    ~addUserFir();
signals:
    void Reselect();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_close_clicked();
       void keyPressEvent(QKeyEvent *e);

        void closeEvent(QCloseEvent *event);

private:
    Ui::addUserFir *ui;
    QSqlQuery qry1;
    QMdiArea *MdiArea;
    int id1;
};

#endif // ADDUSERFIR_H
