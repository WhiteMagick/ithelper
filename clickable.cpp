#include "clickable.h"

Clickable::Clickable()
{

}

void Clickable::slotClik()
{

}

void Clickable::mousePressEvent(QMouseEvent *event)
{


    emit clicked(this->text());

}
