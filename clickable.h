#ifndef CLICKABLE_H
#define CLICKABLE_H

#include <QObject>
#include <QLabel>
class Clickable : public QLabel
{   Q_OBJECT
public:
    Clickable();


signals:
   void clicked(QString st);

public slots:

   void slotClik();
   void mousePressEvent(QMouseEvent* event);


        };

#endif // CLICKABLE_H
