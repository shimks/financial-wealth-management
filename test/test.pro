include(../georgerepo.pri)

QT += testlib
QT += gui
CONFIG += testcase

TEMPLATE = app

SOURCES += \
    main.cpp \
    example-testcase.cpp

HEADERS += \
    example-testcase.h
