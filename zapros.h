#ifndef ZAPROS_H
#define ZAPROS_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QAxObject>
#include <QUuid>
#include <qt_windows.h>
#include <QSettings>

class Zapros : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSqlQuery qry READ qry WRITE setQry NOTIFY qryChanged)
    Q_PROPERTY(QString str READ str WRITE setStr NOTIFY strChanged)
    QSqlQuery m_qry;
    QString m_str;
    QString query;
    QString hdd;
    QString cpu;
    LONGLONG ozu;
    LONGLONG sizeHDD;
    float cpuz;
    QString ip;
    QString namePC;
    QStringList list;
    QSqlDatabase db1;


public:
    explicit Zapros(QObject *parent = nullptr);



    QSqlQuery qry() const;

    QString str() const;

signals:
    void finished();
    void send(QSqlQuery qry);
    void qryChanged(QSqlQuery qry);
    void strChanged(QString str);
    void sendWMI(QStringList list,int fl);


public slots:
    void run();
    void select();
    void finish();
    void setQry(QSqlQuery qry);
    void setStr(QString str);
    void selectWMI();
    void setName(QString str);
    void setName2(QString str);




};

#endif // ZAPROS_H
