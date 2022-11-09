#include "otdel.h"
#include "ui_otdel.h"
#include "addotdel.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlError>
#include <QKeyEvent>
#include <QAxObject>

Otdel::Otdel(QMdiArea *md,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Otdel)
{
    MdiAreaOtdel = md;
    ui->setupUi(this);
    Login *k=new Login;
    setWindowTitle("Список отделов");
    model = new QSqlTableModel(this,k->db);
    model->setTable("Otdel");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalScrollBar();
    model->setHeaderData(1, Qt::Horizontal, "Название");
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    model->setHeaderData(2, Qt::Horizontal, "Код");
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);

}

Otdel::~Otdel()
{
    delete ui;
}

void Otdel::reselect()
{
model->select();

}

void Otdel::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape){

    }
}

void Otdel::wordRep()
{
    //интерфейс к MSWord
        QAxObject *wordApp = new QAxObject("Word.Application");
        //интерфейс к подобъекту - коллекция скрытых документов

        QAxObject *wordDoc = wordApp->querySubObject("Documents()");
        //создаем новый документ
        QAxObject *newDoc = wordDoc->querySubObject("Add()");

        QAxObject *sel = wordApp->querySubObject("Selection()");
        char FontFace[100] = "Times New Roman";
        int size = 13;
                QAxObject *Font = sel->querySubObject("Font");
                Font->dynamicCall("SetSize", size);
                Font->dynamicCall("SetName", FontFace); // Шрифт сменился

        //альбомная оринентация
        QAxObject * orien = newDoc->querySubObject("PageSetup()");
       orien->setProperty("Orientation","wdOrientLandscape");

        ///Заголовок для входных данных
        QAxObject *rangeInputData = newDoc->querySubObject("Range()");
        rangeInputData->dynamicCall("SetRange(int, int)", 0, 100);
        rangeInputData->setProperty("Text", "Приложение № ");
        rangeInputData->dynamicCall("InsertParagraphAfter()");
        QAxObject *rangeInputData1 = newDoc->querySubObject("Range()");
        rangeInputData1->dynamicCall("SetRange(int, int)", 101, 100);
        rangeInputData1->setProperty("Text", "К приказу Инспекции ");



        //размер, шрифт, выравнивание
//        QAxObject *font_rangeInputData = rangeInputData->querySubObject("Font");
//        font_rangeInputData->setProperty("Size", 13);
//        font_rangeInputData->setProperty("Name", "Times New Roman");
        QAxObject *alignment_rangeInputData = rangeInputData->querySubObject("ParagraphFormat");
        alignment_rangeInputData->setProperty("Alignment", "wdAlignParagraphRight");
        alignment_rangeInputData->setProperty("SpaceAfter", 0);

        //отступ
        rangeInputData->dynamicCall("InsertParagraphAfter()");
        rangeInputData->dynamicCall("InsertParagraphAfter()");

        ///Заголовок для 1 таблицы
        QAxObject *rangeNameT1 = newDoc->querySubObject("Range()");
        rangeNameT1->dynamicCall("SetRange(int, int)", 101, 200);
        rangeNameT1->setProperty("Text", "Название отдела");

        //выравнивание
        QAxObject *alignment_rangeNameT1 = rangeNameT1->querySubObject("ParagraphFormat");
        alignment_rangeNameT1->setProperty("Alignment", "wdAlignParagraphLeft");

        //таблица 1
        QAxObject *tables = newDoc->querySubObject("Tables()");
        QAxObject *rangeTable1 = newDoc->querySubObject("Range()");
        rangeTable1->dynamicCall("SetRange(int, int)", 201, 300);
        QAxObject* table1 = tables->querySubObject("Add(Range, NumRows, NumColumns, DefaultTableBehavior, AutoFitBehavior)", rangeTable1->asVariant(), 4, 3, 1, 1);
        //Заполняем таблицу
        //горизонтальные заголовки
        QAxObject *currentCell = table1->querySubObject("Cell(Row, Column)", 1, 1);
        QAxObject *rangeCurrentCell = currentCell->querySubObject("Range()");
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Наименование");
        currentCell = table1->querySubObject("Cell(Row, Column)", 1, 2);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Формула");

        currentCell = table1->querySubObject("Cell(Row, Column)", 1, 3);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Значение");
        //вертикальные заголовки
        for (int i = 0; i < 3; i++)
        {
            currentCell = table1->querySubObject("Cell(Row, Column)", i+2, 1);
            rangeCurrentCell = currentCell->querySubObject("Range()");
            QString temp = "Переменная" + QString::number(i);
            rangeCurrentCell->dynamicCall("InsertAfter(Text)", temp);
        }
        //формулы


        //делаем ворд видимым
        wordApp->setProperty("Visible", true);
       // delete font_rangeCel;
        delete rangeCurrentCell;
        //delete font_range1;
        delete currentCell;
        delete table1;
        delete rangeTable1;
        delete tables;
        delete alignment_rangeNameT1;
        delete rangeNameT1;
        delete alignment_rangeInputData;
        //delete font_rangeInputData;
        delete rangeInputData;
        delete orien;
        delete newDoc;
        delete wordDoc;
        delete wordApp;
}

void Otdel::on_pushButton_clicked()
{
    AddOtdel *otdel1 = new AddOtdel(0,MdiAreaOtdel);
    QMdiSubWindow *subWindow = MdiAreaOtdel->addSubWindow(otdel1);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 590, 183);
    subWindow->show();
     connect(otdel1,SIGNAL(Reselect()),this,SLOT(reselect()));

}

void Otdel::on_pushButton_2_clicked()
{

    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    QSqlQuery qryDelSubdiv;
    qryDelSubdiv.prepare("DELETE from Otdel where id=(:id);");
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

void Otdel::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex currentDiscount  = ui->tableView->currentIndex();                                       //получение значения первого столбца таблицы.
   id = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0).toInt();
    AddOtdel *otdel1 = new AddOtdel(id,MdiAreaOtdel);
    QMdiSubWindow *subWindow = MdiAreaOtdel->addSubWindow(otdel1);
    QScreen *screen = qApp->primaryScreen();
    int x = screen->virtualGeometry().width() / 2 - subWindow->width() / 2;
    int y = screen->virtualGeometry().height() / 2 - subWindow->height() / 2-150;
    subWindow->setGeometry(x, y, 590, 183);
    subWindow->show();
    connect(otdel1,SIGNAL(Reselect()),this,SLOT(reselect()));

}

void Otdel::odbcExcel()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","xlsdb");
           db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};FIL={MS Excel};READONLY=FALSE;DBQ=C:\\progi\\fir.xlsx;");
    QSqlQuery qry;
           if(!db.open())
               qDebug() << "Cannot open database:" << db.lastError();
           QSqlQuery query(db);
           db.transaction();
           query.exec("SELECT * FROM [list1$] where FIO like 'Наседкина Татьяна Александровна' and qry like '%SELECT%';");
            qDebug() <<  query.QSqlQuery::lastError();

           while (query.next()) {
               //QString name = query.value(0).toString();
               qDebug() << query.value(0).toString()+","+query.value(1).toString()+","+query.value(2).toString()+","+query.value(3).toString()+","+query.value(9).toString();
               qry.prepare("INSERT INTO JurnalFir (Login, FIO, Data_zp, systema, qry) VALUES ( :Logi, :FIO, :Data_zp, :systema, :qry);");
               qry.bindValue(":Logi",query.value(0).toString());
               qry.bindValue(":FIO",query.value(1).toString());
               qry.bindValue(":Data_zp",query.value(5).toString());
               qry.bindValue(":systema",query.value(6).toString());
               qry.bindValue(":qry",query.value(9).toString());
               qry.exec();

             }
           db.QSqlDatabase::commit();
           QSqlDatabase::removeDatabase("xlsdb");
}


