#-------------------------------------------------
#
# Project created by QtCreator 2013-08-22T12:27:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CanFlashing
CONFIG   -= console
#CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    IsoTp/IsoTpFrame/singleframe.cpp \
    IsoTp/IsoTpFrame/firstframe.cpp \
    IsoTp/IsoTpFrame/consecutiveframe.cpp \
    IsoTp/IsoTpFrame/flowcontrolframe.cpp \
    IsoTp/tpreceivetransaction.cpp \
    IsoTp/IsoTpFrame/isotpframe.cpp \
    IsoTp/IsoTpFrame/workingwithframes.cpp \
    IsoTp/tpsendtransaction.cpp \
    debugging.cpp \
    Fudp/FudpMessage/message.cpp \
    Fudp/FudpMessage/proginit.cpp \
    Fudp/FudpMessage/progstatus.cpp \
    Fudp/FudpMessage/proglistrq.cpp \
    Fudp/FudpMessage/proglist.cpp \
    Fudp/FudpMessage/progreadrq.cpp \
    Fudp/FudpMessage/progread.cpp \
    Fudp/devfileinfo.cpp \
    Fudp/FudpMessage/progrm.cpp \
    Fudp/FudpMessage/progrmack.cpp \
    Fudp/FudpMessage/progmrproper.cpp \
    Fudp/FudpMessage/progcreate.cpp \
    Fudp/FudpMessage/progwrite.cpp \
    Fudp/FudpMessage/progcreateack.cpp \
    Fudp/FudpMessage/progmrproperack.cpp \
    Fudp/canprog.cpp \
    Fudp/FudpMessage/paramsetrq.cpp \
    Fudp/FudpMessage/paramsetack.cpp \
    Fudp/FudpMessage/paramrmrq.cpp \
    Fudp/FudpMessage/paramrmack.cpp \
    Fudp/FudpMessage/workingwithfudpmessage.cpp \
    IsoTp/isotpcommunicator.cpp \
    PropStore/propstore.cpp \
    PropStore/simplefilepropstore.cpp \
    Fudp/FudpMessage/progsubmit.cpp \
    Fudp/FudpMessage/progbroadcastanswer.cpp \
    Fudp/FudpMessage/progfirmcorrupt.cpp \
    form.cpp \
    Fudp/FudpMessage/progwriteack.cpp \
    Fudp/FudpMessage/progsubmitack.cpp \
    canprogworker.cpp \
    qtCanLib/canframe.cpp \
    qtCanLib/CanNick/workingwithmessage.cpp \
    qtCanLib/CanNick/candriver.cpp \
    qtCanLib/cannick.cpp \
    qtCanLib/socketcan.cpp \
    qtCanLib/socketcan/sktcan.cpp \
    qtCanLib/socketcan/cansendqueue.cpp \
    qtDoodahLib/queues.cpp \
    processmanager.cpp \
    UpdateManager.cpp \
    SharedCanUpdateManager.cpp \
    ExclusiveCanUpdateManager.cpp \
    PropStore/constloaderstore.cpp

HEADERS += \
    IsoTp/IsoTpFrame/singleframe.h \
    IsoTp/IsoTpFrame/firstframe.h \
    IsoTp/IsoTpFrame/consecutiveframe.h \
    IsoTp/IsoTpFrame/flowcontrolframe.h \
    IsoTp/tpreceivetransaction.h \
    IsoTp/tpreceivetransaction.h \
    IsoTp/IsoTpFrame/isotpframe.h \
    IsoTp/IsoTpFrame/workingwithframes.h \
    IsoTp/tpsendtransaction.h \
    debugging.h \
    Fudp/FudpMessage/message.h \
    Fudp/FudpMessage/proginit.h \
    Fudp/FudpMessage/progstatus.h \
    Fudp/FudpMessage/proglistrq.h \
    Fudp/FudpMessage/proglist.h \
    Fudp/FudpMessage/progreadrq.h \
    Fudp/FudpMessage/progread.h \
    Fudp/devfileinfo.h \
    Fudp/FudpMessage/progrm.h \
    Fudp/FudpMessage/progrmack.h \
    Fudp/FudpMessage/progmrproper.h \
    Fudp/FudpMessage/progmrproperack.h \
    Fudp/FudpMessage/progcreate.h \
    Fudp/FudpMessage/progwrite.h \
    Fudp/FudpMessage/progcreateack.h \
    Fudp/canprog.h \
    Fudp/FudpMessage/paramsetrq.h \
    Fudp/FudpMessage/paramsetack.h \
    Fudp/FudpMessage/paramrmrq.h \
    Fudp/FudpMessage/paramrmack.h \
    Fudp/FudpMessage/workingwithfudpmessage.h \
    IsoTp/isotpcommunicator.h \
    PropStore/propstore.h \
    PropStore/simplefilepropstore.h \
    Fudp/FudpMessage/progsubmit.h \
    Fudp/FudpMessage/progbroadcastanswer.h \
    Fudp/FudpMessage/progfirmcorrupt.h \
    form.h \
    Fudp/FudpMessage/progwriteack.h \
    Fudp/FudpMessage/progsubmitack.h \
    Log/singletone.h \
    Log/logwriter.h \
    Log/singletone.h \
    Log/logwriter.h \
    canprogworker.h \
    qtCanLib/canframe.h \
    qtCanLib/can.h \
    qtCanLib/CanNick/workingwithmessage.h \
    qtCanLib/CanNick/candriver.h \
    qtCanLib/cannick.h \
    qtCanLib/dummycan.h \
    qtCanLib/socketcan/sktcan.h \
    qtCanLib/socketcan/cansendqueue.h \
    qtCanLib/socketcan.h \
    qtDoodahLib/queues.h \
    cDoodahLib/packing.h \
    cDoodahLib/masqarade.h \
    cDoodahLib/lowlevel.h \
    processmanager.h \
    UpdateManager.h \
    SharedCanUpdateManager.h \
    ExclusiveCanUpdateManager.h \
    PropStore/constloaderstore.h

QMAKE_CXXFLAGS += -std=c++0x

FORMS += form.ui

#For Monitor 5 use: MONITOR_5, LIB_CAN_NICK
#For Linux use: LIB_SOCKET_CAN
