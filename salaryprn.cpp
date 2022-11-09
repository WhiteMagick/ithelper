#include "salaryprn.h"
#include "ui_salaryprn.h"
#include "login.h"
#include <QKeyEvent>

SalaryPrn::SalaryPrn(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SalaryPrn)
{
    ui->setupUi(this);
    setWindowTitle("Справочник Принтеров");
     Login *k = new Login;
    SalPrn = md;
    model = new QSqlTableModel(this,k->db);
    model->setTable("SalaryPrn");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("Name"));
    proxi = new QSortFilterProxyModel(this);
    proxi->setSourceModel(model);
    ui->tableView->setModel(proxi);
    ui->tableView->setSortingEnabled(true);
    //ui->tableView->hideColumn(0);
    ui->tableView->setColumnWidth(0, 60);
    ui->tableView->setColumnWidth(1,400);
   ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);

   model2 = new QSqlTableModel(this,k->db);
   model2->setTable("SalaryMakerPRN");
   model2->setEditStrategy(QSqlTableModel::OnRowChange);
   model2->select();
   model2->setHeaderData(1, Qt::Horizontal, tr("Name"));
   proxi2 = new QSortFilterProxyModel(this);
   proxi2->setSourceModel(model2);
   ui->tableView_maker->setModel(proxi2);
   ui->tableView_maker->setSortingEnabled(true);
   //ui->tableView->hideColumn(0);
   ui->tableView_maker->setColumnWidth(0, 60);
   ui->tableView_maker->setColumnWidth(1,400);
   ui->tableView_maker->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);

}

SalaryPrn::~SalaryPrn()
{
    delete ui;
}

void SalaryPrn::on_pushButton_Add_clicked()
{
    if(ui->tabWidget->currentIndex()==0){
    model->insertRow(model->rowCount());
     }else if(ui->tabWidget->currentIndex()==1){
        model2->insertRow(model2->rowCount());
    }
}

void SalaryPrn::on_pushButton_dell_clicked()
{
   if(ui->tabWidget->currentIndex()==0){
    int row = ui->tableView->currentIndex().row();
   if(row>=0){
       model->removeRow(row);
   }else{
       qDebug("ska");
   }
   }else if(ui->tabWidget->currentIndex()==1){
       int row = ui->tableView_maker->currentIndex().row();
      if(row>=0){
          model2->removeRow(row);
      }else{
          qDebug("ska");
      }
   }


}

void SalaryPrn::on_pushButton_clicked()
{
    if(ui->tabWidget->currentIndex()==0){
    proxi->setFilterWildcard("*"+ui->lineEdit->text()+"*");
    proxi->setFilterKeyColumn(1);
    }else if(ui->tabWidget->currentIndex()==1){
        proxi2->setFilterWildcard("*"+ui->lineEdit->text()+"*");
        proxi2->setFilterKeyColumn(1);
    }
}

void SalaryPrn::on_pushButton_Upd_clicked()
{
   if(ui->tabWidget->currentIndex()==0){
    proxi->setFilterWildcard("");
    model->select();
   }else if(ui->tabWidget->currentIndex()==1){
       proxi2->setFilterWildcard("");
       model2->select();
   }
}

void SalaryPrn::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Enter:{
                        if(ui->lineEdit->hasFocus()){
                            if(ui->tabWidget->currentIndex()==0){
                            proxi->setFilterWildcard("*"+ui->lineEdit->text()+"*");
                            proxi->setFilterKeyColumn(1);
                            }else if(ui->tabWidget->currentIndex()==1){
                                proxi2->setFilterWildcard("*"+ui->lineEdit->text()+"*");
                                proxi2->setFilterKeyColumn(1);
                            }
                        }
                        break;
                        }
    case Qt::Key_Escape:{

                        break;
                        }
    }


}

void SalaryPrn::on_tabWidget_currentChanged(int index)
{



}
