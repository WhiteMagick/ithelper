#-------------------------------------------------
#
# Project created by QtCreator 2019-04-12T19:56:29
#
#-------------------------------------------------

QT       += core gui sql axcontainer printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE = icon.rc

TARGET = IT_Work
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    addkartridj.cpp \
    addkolonki.cpp \
    adduserfir.cpp \
    checkboxmodel.cpp \
    delegatefromkolonky.cpp \
    delegatekartr.cpp \
    fir.cpp \
    kartridj.cpp \
    kolonki.cpp \
        main.cpp \
        mainwindow.cpp \
    login.cpp \
    navi.cpp \
    comp.cpp \
    monitor.cpp \
    os.cpp \
    otdel.cpp \
    perepis.cpp \
    addcomp.cpp \
    addotdel.cpp \
    addos.cpp \
    addmon.cpp \
    addperepis.cpp \
    report.cpp \
    search.cpp \
    printer.cpp \
    addprn.cpp \
    userfir.cpp \
    viborotdela.cpp \
    zapros.cpp \
    backup.cpp \
    clickable.cpp \
    simpledelegate.cpp \
    delegateforprinter.cpp \
    asistant.cpp \
    delegateforcomp.cpp \
    monitoringconnect.cpp \
    otherhardware.cpp \
    addotherh.cpp \
    zapchasti.cpp \
    addzapprn.cpp \
    header.cpp \
    model.cpp \
    tableview.cpp \
    salaryprn.cpp \
    defhistory.cpp \
    delegatefromzap.cpp \
    mdiarea.cpp

HEADERS += \
    addkartridj.h \
    addkolonki.h \
    adduserfir.h \
    checkboxmodel.h \
    delegatefromkolonky.h \
    delegatekartr.h \
    fir.h \
    kartridj.h \
    kolonki.h \
        mainwindow.h \
    login.h \
    navi.h \
    comp.h \
    monitor.h \
    os.h \
    otdel.h \
    perepis.h \
    addcomp.h \
    addotdel.h \
    addos.h \
    addmon.h \
    addperepis.h \
    report.h \
    search.h \
    printer.h \
    addprn.h \
    userfir.h \
    viborotdela.h \
    zapros.h \
    backup.h \
    clickable.h \
    simpledelegate.h \
    delegateforprinter.h \
    asistant.h \
    delegateforcomp.h \
    monitoringconnect.h \
    otherhardware.h \
    addotherh.h \
    zapchasti.h \
    addzapprn.h \
    header.h \
    model.h \
    tableview.h \
    salaryprn.h \
    defhistory.h \
    delegatefromzap.h \
    mdiarea.h

FORMS += \
    addkartridj.ui \
    addkolonki.ui \
    adduserfir.ui \
    fir.ui \
    kartridj.ui \
    kolonki.ui \
        mainwindow.ui \
    login.ui \
    navi.ui \
    comp.ui \
    monitor.ui \
    os.ui \
    otdel.ui \
    perepis.ui \
    addcomp.ui \
    addotdel.ui \
    addos.ui \
    addmon.ui \
    addperepis.ui \
    search.ui \
    printer.ui \
    addprn.ui \
    userfir.ui \
    viborotdela.ui \
    backup.ui \
    asistant.ui \
    monitoringconnect.ui \
    otherhardware.ui \
    addotherh.ui \
    zapchasti.ui \
    addzapprn.ui \
    salaryprn.ui \
    defhistory.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img/img.qrc

DISTFILES += \
    img/transport.png \
    img/plus.png
