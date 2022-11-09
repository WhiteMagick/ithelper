#ifndef ABSTTABMODEL_H
#define ABSTTABMODEL_H

#include <QtWidgets>
#include <QSqlTableModel>

class AbstTabModel : public QSqlTableModel
{
public:
    AbstTabModel(QObject *pobj);
    QVariant data(const QModelIndex &index, int role) const;
   /* Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
    Q_INVOKABLE virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0*/;
};

#endif // ABSTTABMODEL_H
