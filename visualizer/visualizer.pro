#-------------------------------------------------
#
# Project created by QtCreator 2013-02-01T15:07:11
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visualizer
TEMPLATE = app


SOURCES += \
    mainwindow.cpp \
    main.cpp \
    heessystemscene.cpp \
    heesitemwidget.cpp \
    heesgraphicsitem.cpp \
    derivedattributes.cpp \
    arrowitem.cpp \
    ../tinyxml/tinyxmlparser.cpp \
    ../tinyxml/tinyxmlerror.cpp \
    ../tinyxml/tinyxml.cpp \
    ../tinyxml/tinystr.cpp \
    sensorlist.cpp \
    commandsmodel.cpp

HEADERS  += \
    mainwindow.h \
    heessystemscene.h \
    heesitemwidget.h \
    heesgraphicsitem.h \
    derivedattributes.h \
    arrowitem.h \
    ../tinyxml/tinyxml.h \
    ../tinyxml/tinystr.h \
    sensorlist.h \
    commandsmodel.h

FORMS    += \
    mainwindow.ui
