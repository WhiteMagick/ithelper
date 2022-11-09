#ifndef POTOKY_H
#define POTOKY_H

#include <QObject>
#include "mainwindow.h"
#include <QDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QAction>
#include <QWidget>
#include <QObject>
#include <QThread>


class Potoky : public QObject
{
    Q_OBJECT
public:
    explicit Potoky(QObject *parent = nullptr);

QMdiArea *md;
signals:
 void finished();

public slots:
void setMdiarea(QMdiArea *m);
void run();
};

#endif // POTOKY_H
