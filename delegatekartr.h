#ifndef DELEGATEKARTR_H
#define DELEGATEKARTR_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>
#include <QString>
#include <QStyleOptionViewItem>

class delegatekartr : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit delegatekartr(QObject *parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
     void setEditorData(QWidget *editor, const QModelIndex &index) const;
       void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
       void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
       void paint(QPainter* painter,
                   const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void idcombo(int);
signals:


};

#endif // DELEGATEKARTR_H
