#-------------------------------------------------
#
# Project created by QtCreator 2014-04-22T15:41:47
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_propstoretest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_propstoretest.cpp \
    ../PropStore/FilePropStore.cpp \
    ../PropStore/PropStore.cpp \
    ../PropStore/ConstLoaderStore.cpp \
    ../PropStore/CompositePropStore.cpp \
    ../PropStore/PropStoreDecorators.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../PropStore/FilePropStore.h \
    ../PropStore/PropStore.h \
    ../PropStore/ConstLoaderStore.h \
    ../PropStore/CompositePropStore.h \
    ../PropStore/PropStoreDecorators.h
