#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QSet>
#include <QFileInfo>
#include <QHashFunctions>
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    QSqlDatabase db;
public slots:


private slots:

         void on_LoginButton_clicked();
         void saveSetting();
         void loadSetting();
         void updateCheck(const QDir &dir, const QDir &dir2);


private:
    Ui::Login *ui;
      QMessageBox *msg;
};

#endif // LOGIN_H
