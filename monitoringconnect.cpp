#include "monitoringconnect.h"
#include "ui_monitoringconnect.h"

MonitoringConnect::MonitoringConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonitoringConnect)
{
    ui->setupUi(this);
}

MonitoringConnect::~MonitoringConnect()
{
    delete ui;
}
