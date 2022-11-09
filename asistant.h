#ifndef ASISTANT_H
#define ASISTANT_H

#include <QDialog>
#include <QMdiArea>

namespace Ui {
class Asistant;
}

class Asistant : public QDialog
{
    Q_OBJECT

public:
    explicit Asistant(QMdiArea *md,QWidget *parent = nullptr);
    ~Asistant();



private:
    Ui::Asistant *ui;
};

#endif // ASISTANT_H
