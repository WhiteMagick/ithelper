#ifndef KOLONKI_H
#define KOLONKI_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QKeyEvent>
#include <QMdiArea>

namespace Ui {
class Kolonki;
}

class Kolonki : public QDialog
{
    Q_OBJECT

public:
    explicit Kolonki(QMdiArea *md,QWidget *parent = nullptr);
    ~Kolonki();
    int id;

private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_Dell_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

   void reselect();

    void customMenu(QPoint pos);

    void history();

    void keyPressEvent(QKeyEvent *e);

    void on_pushButton_hide_clicked();

signals:
    void setGUID(QString g);

private:
    Ui::Kolonki *ui;
    QSqlTableModel *model;
    QMdiArea *MdiAreaKolonki;
    QSqlQuery search;
    QSqlQueryModel *qryModel;
    QSqlQuery qry;
    int click;
};

#endif // KOLONKI_H
