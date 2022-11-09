#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include "navi.h"
#include "comp.h"
#include "monitor.h"
#include "os.h"
#include "otdel.h"
#include "perepis.h"
#include "addcomp.h"
#include "addotdel.h"
#include "addmon.h"
#include "addperepis.h"
#include "search.h"
#include "printer.h"
#include "backup.h"
#include "otherhardware.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login l;
    l.show();

    return a.exec();
}
