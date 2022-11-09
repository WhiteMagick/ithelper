#ifndef FIR_H
#define FIR_H

#include <QDialog>
#include <QSqlQuery>
#include <QMdiArea>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class Fir;
}

class Fir : public QDialog
{
    Q_OBJECT

public:
    explicit Fir(QMdiArea *md,QWidget *parent = nullptr);
    ~Fir();

private slots:
    void loadUsers();

    void on_selectFile_clicked();

    void on_createJurnal_clicked();

    void on_cleanTbl_clicked();

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::Fir *ui;
    QMdiArea *MdiAreaFir;
};

#endif // FIR_H
