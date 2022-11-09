#ifndef MDIAREA_H
#define MDIAREA_H

#include <QMdiArea>
#include <QPixmap>
#include <QPainter>

class mdiarea : public QMdiArea
{
public:
    mdiarea(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
private:
    // Store the logo image.
    QPixmap m_pixmap;
};


#endif // MDIAREA_H
