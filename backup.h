#ifndef BACKUP_H
#define BACKUP_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMdiArea>
#include <QtWidgets>
#include "login.h"
namespace Ui {
class backup;
}

class backup : public QDialog
{
    Q_OBJECT

public:
    explicit backup(QMdiArea *md,QWidget *parent = nullptr);

    ~backup();
    QVBoxLayout *vboxleft;
    QHBoxLayout *hboxMain;
    QVBoxLayout *vboxright;
    QHBoxLayout *hboxBtnHight;
    QHBoxLayout *hboxBtnLow;
    QPushButton *close;
    QLabel *text;
    QLabel *text1;
    QPushButton *btnAdd;
     QTableView *view;
     QFileSystemModel *model;
     QDir Rootdir;
     QPushButton* dell;
      QFrame *wgt;

signals:
      void repaint();

public slots:

    void createWindow();

    void addDisk();

    void test();

    void restoreDisk();

    void labelClik(QString st);

    void moveTo(const QModelIndex &index);

    void dellDisk();

    void refresh(const QString &d);

   void slotButtonClicked();

   void keyPressEvent(QKeyEvent *e);

    //void closeEvent(QCloseEvent *event);


private:
    Ui::backup *ui;
    QMdiArea *MdiArea;

};

#endif // BACKUP_H
