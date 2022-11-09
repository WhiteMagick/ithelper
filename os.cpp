#include "os.h"
#include "ui_os.h"
#include "addos.h"
#include "addcomp.h"
#include "simpledelegate.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlError>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeyEvent>

OS::OS(int id,QMdiArea *md, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OS)
{
    MdiAreaOS = md;
    id2=id;
    ui->setupUi(this);
    Login *k=new Login;
    if(id==0){
    setWindowTitle("Список операционных систем");
    model = new QSqlTableModel(this,k->db);
    model->setTable("OS");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalScrollBar();
    model->setHeaderData(1, Qt::Horizontal, "Название");
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
     model->setHeaderData(2, Qt::Horizontal, "Разрядность");
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);}
else {
    setWindowTitle("Выберите операционную систему");
    model = new QSqlTableModel(this,k->db);
    model->setTable("OS");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new SimpleDelegate(ui->tableView));
    ui->tableView->viewport()->setAttribute(Qt::WA_Hover);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalScrollBar();
    model->setHeaderData(1, Qt::Horizontal, "Название");
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
     model->setHeaderData(2, Qt::Horizontal, "Разрядность");
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->pushButton->setHidden(true);
    ui->pushButton_2->setHidden(true);
}
}

OS::~OS()
{
    delete ui;
}

void OS::on_pushButton_clicked()
{
    AddOs *os = new AddOs(0,MdiAreaOS);
    QMdiSubWindow *subWindow = MdiAreaOS->addSubWindow(os);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 590, 183);
    subWindow->show();
    connect(os,SIGNAL(Reselect()),this,SLOT(reselect()));

}

void OS::on_pushButton_2_clicked()
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    QSqlQuery qryDelSubdiv;
    qryDelSubdiv.prepare("DELETE from OS where id=(:id);");
    qryDelSubdiv.bindValue(":id",id);
    QMessageBox::StandardButton msg;
    msg=QMessageBox::question(this,"Внимание","Удаление записи возможно только в том случае если запись не связана с другими таблицами.",QMessageBox::Yes|QMessageBox::No);
    if(msg == QMessageBox::Yes){
        if(qryDelSubdiv.exec()){
            model->select();
        }else{
           QMessageBox::about(this,"Error",qryDelSubdiv.lastError().text());
        }
    }
}

void OS::on_tableView_doubleClicked(const QModelIndex &index)
{

    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
   if(id2==1){                                         //условие если вызвали из добавления компа
       emit vibor(id);
       MdiAreaOS->closeActiveSubWindow();

   }
   else{
    AddOs *os = new AddOs(id,MdiAreaOS);
    QMdiSubWindow *subWindow = MdiAreaOS->addSubWindow(os);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 590, 183);
    subWindow->show();
    connect(os,SIGNAL(Reselect()),this,SLOT(reselect()));
}

}

void OS::reselect()
{
    model->select();
}

void OS::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}


