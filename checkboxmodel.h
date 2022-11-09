#ifndef CHECKBOXMODEL_H
#define CHECKBOXMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QPersistentModelIndex>
#include <QSet>

class checkboxModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit checkboxModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,int role);



private:
       QMap<int, Qt::CheckState> check_state_map;
       // QMap<QPersistentModelIndex, QVariant> modelDataMap;

};

#endif // CHECKBOXMODEL_H
