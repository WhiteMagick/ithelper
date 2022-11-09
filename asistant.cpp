#include "asistant.h"
#include "ui_asistant.h"

Asistant::Asistant(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Asistant)
{
    ui->setupUi(this);
    setWindowTitle("Справка");
   QPalette pal;
   pal.setColor(QPalette::Background,Qt::white);
   setPalette(pal);
   setAutoFillBackground(true);
}

Asistant::~Asistant()
{
    delete ui;
}
