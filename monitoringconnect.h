#ifndef MONITORINGCONNECT_H
#define MONITORINGCONNECT_H

#include <QDialog>

namespace Ui {
class MonitoringConnect;
}

class MonitoringConnect : public QDialog
{
    Q_OBJECT

public:
    explicit MonitoringConnect(QWidget *parent = nullptr);
    ~MonitoringConnect();

private:
    Ui::MonitoringConnect *ui;
};

#endif // MONITORINGCONNECT_H
