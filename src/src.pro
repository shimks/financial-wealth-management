#-------------------------------------------------
#
# Project created by QtCreator 2018-10-22T00:01:56
#
#-------------------------------------------------

include(../georgerepo.pri)
QT       += sql
TARGET = TG
TEMPLATE = app
RC_ICONS = TG320Logo.ico

SOURCES += \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS +=

HEADERS +=
