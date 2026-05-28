//program_final

QT += core gui widgets multimedia

CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ayam
TEMPLATE = app

RC_ICONS = logo.ico

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui
