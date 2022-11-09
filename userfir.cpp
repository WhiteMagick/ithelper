#include "userfir.h"
#include "ui_userfir.h"
#include "login.h"
#include "adduserfir.h"
#include <QScreen>
#include <QMenu>
#include <QCoreApplication>
#include <QMdiSubWindow>

UserFIR::UserFIR(QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserFIR)
{
    ui->setupUi(this);
    firU = md;
    setWindowTitle("Список пользователей ФИР");

    qryModel=new QSqlQueryModel(this);
    qry.prepare("select id, login, FIO, post, sub, d_reg, d_out, st, face_link from UserFir;");
    qry.exec();
    qryModel->setQuery(qry);
    qryModel->setHeaderData(1, Qt::Horizontal, "Логин");
    qryModel->setHeaderData(2, Qt::Horizontal, "ФИО");
    qryModel->setHeaderData(3, Qt::Horizontal, "Должность");
    qryModel->setHeaderData(4, Qt::Horizontal, "Подразделение");
    qryModel->setHeaderData(5, Qt::Horizontal, "Дата рег.");
    qryModel->setHeaderData(6, Qt::Horizontal, "Дата откл.");
    qryModel->setHeaderData(7, Qt::Horizontal, "Статус");



    proxi = new QSortFilterProxyModel(this);
    proxi->setSourceModel(qryModel);
     ui->tableView->setModel(proxi);
     ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
     connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenu(QPoint)));
     ui->tableView->horizontalScrollBar();
     ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
     ui->tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
     ui->tableView->hideColumn(0);
     ui->tableView->hideColumn(8);
     ui->tableView->setColumnWidth(1, 100);
     ui->tableView->setColumnWidth(2, 300);
     ui->tableView->setColumnWidth(3, 400);
     ui->tableView->setColumnWidth(4, 400);
     ui->tableView->setColumnWidth(5, 200);
     ui->tableView->setColumnWidth(6, 200);
     ui->tableView->setColumnWidth(7, 100);
    ui->tableView->setSortingEnabled(true);
   ui->comboBox->clear();
   ui->comboBox->addItem("ФИО",QVariant(0));
   ui->comboBox->addItem("Логин",QVariant(1));
   ui->comboBox->addItem("Статус",QVariant(2));
}

UserFIR::~UserFIR()
{
    delete ui;
}

void UserFIR::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
    int id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    QSqlQuery log_prov;
    log_prov.prepare("select user_name from JURNAL where tbl = 'UserFir' AND Link = :link;");
    log_prov.bindValue(":link",id);
    log_prov.exec();
    log_prov.next();
    if(log_prov.value(0).toString().length()>0){
        QMessageBox::warning(this,"Error","В данный момент запись используется пользователем "+ log_prov.value(0).toString());
    }else {

    addUserFir *user = new addUserFir(id,firU);
    QMdiSubWindow *subWindow = firU->addSubWindow(user);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 580, 310);
    subWindow->show();
     connect(user,SIGNAL(Reselect()),this,SLOT(Reselect()));
     QSqlQuery log;
     log.prepare("exec addLog :tbl, :link;");
     log.bindValue(":tbl","UserFir");
     log.bindValue(":link",id);
     log.exec();
    }

}

void UserFIR::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}

void UserFIR::on_pushButton_add_clicked()
{
    addUserFir *user = new addUserFir(0,firU);
    QMdiSubWindow *subWindow = firU->addSubWindow(user);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 580, 310);
    subWindow->show();
     connect(user,SIGNAL(Reselect()),this,SLOT(Reselect()));
}

void UserFIR::on_pushButton_del_clicked()
{

}

void UserFIR::on_pushButton_find_clicked()
{
    switch (ui->comboBox->currentIndex()) {
    case 0:{
        proxi->setFilterWildcard("*"+ui->lineEdit->text()+"*");
        proxi->setFilterKeyColumn(2);
        break;
        }
    case 1: {
        proxi->setFilterWildcard("*"+ui->lineEdit->text()+"*");
        proxi->setFilterKeyColumn(1);
        break;
            }
    case 2:{
        proxi->setFilterWildcard("*"+ui->lineEdit->text()+"*");
        proxi->setFilterKeyColumn(7);
        break;
            }
    }


}

void UserFIR::customMenu(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *op = new QAction(tr("Сформировать журнал ФИР"),this);
    connect(op,SIGNAL(triggered()),this,SLOT(CreateJurnal()));
    menu->addAction(op);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

}

void UserFIR::CreateJurnal()
{

}

void UserFIR::on_comboBox_activated(int index)
{
    if(index==2){
        ui->lineEdit->setPlaceholderText("1-вкл, 2 - откл, 0 - хз");
    }
    else {
        ui->lineEdit->setPlaceholderText("");
    }
}

void UserFIR::Reselect()
{
    qry.exec();
    qryModel->setQuery(qry);

}
