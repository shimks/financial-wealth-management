QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/sidebartabstyle.cpp \
    $$PWD/src/networthtab.cpp \
    $$PWD/src/stock.cpp \
    $$PWD/src/adddialog.cpp \
    $$PWD/src/budgettrackertab.cpp \
    $$PWD/src/budgettarget.cpp \
    $$PWD/src/addexpense.cpp \
    $$PWD/src/popup_window.cpp

HEADERS += \
    $$PWD/src/mainwindow.h \
    $$PWD/src/sidebartabstyle.h \
    $$PWD/src/networthtab.h \
    $$PWD/src/stock.h \
    $$PWD/src/adddialog.h \
    $$PWD/src/popup_window.h \
    $$PWD/src/budgettrackertab.h \
    $$PWD/src/budgettarget.h \
    $$PWD/src/addexpense.h

FORMS += \
	$$PWD/src/adddialog.ui \
        $$PWD/src/popup_window.ui \
        $$PWD/src/budgettarget.ui \
        $$PWD/src/addexpense.ui
