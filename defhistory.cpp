#include "defhistory.h"
#include "ui_defhistory.h"
#include <QKeyEvent>

defHistory::defHistory(QMdiArea *md, int id,int idP, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::defHistory)
{
    MdiAreaHist = md;
    id1= id;
    idPrn = idP;
    ui->setupUi(this);
    search = new QPushButton;
    connect(search,SIGNAL(clicked()),this,SLOT(onClick_search()));
    search->setText("Поиск");
    search->setStyleSheet("font-size: 14px;"
                          "min-width: 100;");
    dateF = new QDateEdit;
    dateL = new QDateEdit;
    dateF->setDate(QDate(QDate::currentDate().year(),01,01));
    dateL->setDate(QDate::currentDate());
    if(id1==0&&idPrn==0){
    createHistZap();
    }else{
     createHistPoZap();
    }
}

defHistory::~defHistory()
{
    delete ui;
}

void defHistory::createHistZap()
{
    setWindowTitle("История запчастей");
    vboxmain = new QVBoxLayout;
    hboxUP = new QHBoxLayout;
    setLayout(vboxmain);
    vboxmain->addLayout(hboxUP);
    hboxUP->addWidget(dateF);
    hboxUP->addWidget(dateL);
    text = new QLabel;
    text->setText("Поиск по ИД");
    hboxUP->addWidget(text);
    searchLn = new QLineEdit;
    hboxUP->addWidget(searchLn);
    hboxUP->addWidget(search);
    view = new QTableView;
    qmodel = new QSqlQueryModel(this);
    QSqlQuery qry;
    qry.prepare("Select i.id, i.idPrn, z.name, pn.model, i.dateInst, commet FROM InstallZap i "
    "left join Printer p on p.id = i.idPrn "
    "inner join SalaryPrn pn on pn.id = p.model "
    "left join ZapchastiPrn z on z.id = i.idZap "
    "where dateInst BETWEEN :dF AND :dL;");

    qry.bindValue(":dF",dateF->date().toString("dd.MM.yyyy"));
    qry.bindValue(":dL",dateL->date().toString("dd.MM.yyyy"));
    qry.exec();
    qry.next();
    qmodel->setQuery(qry);
    proxi = new QSortFilterProxyModel(this);
    proxi->setSourceModel(qmodel);
    view->setModel(proxi);
    view->setSortingEnabled(true);
    view->hideColumn(0);
    view->hideColumn(1);
    vboxmain->addWidget(view);

}

void defHistory::createHistPoZap()
{
    setWindowTitle("История запчасти");
    vboxmain = new QVBoxLayout;
    setLayout(vboxmain);
    view = new QTableView;
    qmodel = new QSqlQueryModel(this);
    if(idPrn==0){
    QSqlQuery qry;
    qry.prepare("Select i.id, i.idPrn, z.name, pn.model, i.dateInst, commet FROM InstallZap i "
                "left join Printer p on p.id = i.idPrn "
                "inner join SalaryPrn pn on pn.id = p.model "
                "left join ZapchastiPrn z on z.id = i.idZap "
                "where i.idZap = :id AND (dateInst BETWEEN :dF AND :dL);");
    qry.bindValue(":id",id1);
    qry.bindValue(":dF",dateF->date().toString("dd.MM.yyyy"));
    qry.bindValue(":dL",dateL->date().toString("dd.MM.yyyy"));
    qry.exec();
    qry.next();
    qmodel->setQuery(qry);
    }else{
        QSqlQuery qry;
        qry.prepare("Select i.id, i.idPrn, z.name, pn.model, i.dateInst, commet FROM InstallZap i"
                    "left join Printer p on p.id = i.idPrn "
                    "inner join SalaryPrn pn on pn.id = p.model "
                    "left join ZapchastiPrn z on z.id = i.idZap "
                    " where i.idPrn = :id AND (dateInst BETWEEN :dF AND :dL);");
        qry.bindValue(":id",idPrn);
        qry.bindValue(":dF",dateF->date().toString("dd.MM.yyyy"));
        qry.bindValue(":dL",dateL->date().toString("dd.MM.yyyy"));
        qry.exec();
        qry.next();
        qmodel->setQuery(qry);
    }

    proxi = new QSortFilterProxyModel(this);
    proxi->setSourceModel(qmodel);
    view->setModel(proxi);
   // view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    view->setSortingEnabled(true);
    view->hideColumn(0);
    view->hideColumn(1);
    vboxmain->addWidget(view);
}

void defHistory::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}

void defHistory::onClick_search()
{
    QSqlQuery qry;
    if(searchLn->text().length()==0){
        qry.prepare("Select i.id, i.idPrn, z.name, pn.model, i.dateInst, commet FROM InstallZap i "
                    "left join Printer p on p.id = i.idPrn "
                    "inner join SalaryPrn pn on pn.id = p.model "
                    "left join ZapchastiPrn z on z.id = i.idZap "
                    "where dateInst BETWEEN :dF AND :dL;");
        qDebug()<<"0";
    }else{
    qry.prepare("Select i.id, i.idPrn, z.name, pn.model, i.dateInst, commet FROM InstallZap i "
                "left join Printer p on p.id = i.idPrn "
                "inner join SalaryPrn pn on pn.id = p.model "
                "left join ZapchastiPrn z on z.id = i.idZap "
                "where i.idZap = :id AND (dateInst BETWEEN :dF AND :dL);");
    qry.bindValue(":id",searchLn->text());
    }
    qry.bindValue(":dF",dateF->date().toString("dd.MM.yyyy"));
    qry.bindValue(":dL",dateL->date().toString("dd.MM.yyyy"));
    qry.exec();
    qry.next();
    qmodel->setQuery(qry);


}

