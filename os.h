#ifndef OS_H
#define OS_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include "login.h"
namespace Ui {
class OS;
}

class OS : public QDialog
{
    Q_OBJECT

public:
    explicit OS(int id, QMdiArea *md, QWidget *parent = nullptr);
    ~OS();
    int id;
    int id2;
signals:
    void vibor(int id);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void reselect();

    void keyPressEvent(QKeyEvent *e);


private:
    Ui::OS *ui;
    QSqlTableModel *model;
    QMdiArea *MdiAreaOS;
};

#endif // OS_H
