#ifndef SALARYPRN_H
#define SALARYPRN_H

#include <QDialog>
#include <QMdiArea>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>
#include <QSortFilterProxyModel>
namespace Ui {
class SalaryPrn;
}

class SalaryPrn : public QDialog
{
    Q_OBJECT

public:
    explicit SalaryPrn(QMdiArea *md,QWidget *parent = nullptr);
    ~SalaryPrn();

private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_dell_clicked();

    void on_pushButton_clicked();

    void on_pushButton_Upd_clicked();

    void keyPressEvent(QKeyEvent *e);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::SalaryPrn *ui;
    QSqlTableModel *model;
     QSqlTableModel *model2;
    QMdiArea *SalPrn;
    QSortFilterProxyModel *proxi;
    QSortFilterProxyModel *proxi2;

};

#endif // SALARYPRN_H
