#ifndef REPORT_H
#define REPORT_H
#include "mainwindow.h"
#include <QObject>
#include <QAction>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
class report : public QObject
{
    Q_OBJECT
public:
    explicit report(QObject *parent = nullptr);
public slots:
    void repPrint();
signals:

private:


};

#endif // REPORT_H
