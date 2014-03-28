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
    qtBlokLib/parser.cpp \
    qtBlokLib/parsers/vds_state.cpp \
    qtBlokLib/parsers/uktol_dd1.cpp \
    qtBlokLib/parsers/tskbm_state.cpp \
    qtBlokLib/parsers/sys_key.cpp \
    qtBlokLib/parsers/mvd_dd.cpp \
    qtBlokLib/parsers/mp_state.cpp \
    qtBlokLib/parsers/mm_coord.cpp \
    qtBlokLib/parsers/mm_alt_long.cpp \
    qtBlokLib/parsers/mco_state.cpp \
    qtBlokLib/parsers/mco_mode.cpp \
    qtBlokLib/parsers/mco_limits.cpp \
    qtBlokLib/parsers/ipd_state.cpp \
    qtBlokLib/parsers/display_state.cpp \
    qtBlokLib/parsers/canblokmessage.cpp \
    qtBlokLib/parsers/console_key.cpp \
    Log/logwriter.cpp

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
    qtBlokLib/parser.h \
    qtBlokLib/parsers/vds_state.h \
    qtBlokLib/parsers/uktol_dd1.h \
    qtBlokLib/parsers/tskbm_state.h \
    qtBlokLib/parsers/trafficlight.h \
    qtBlokLib/parsers/sys_key.h \
    qtBlokLib/parsers/pressure.h \
    qtBlokLib/parsers/mvd_dd.h \
    qtBlokLib/parsers/mp_state.h \
    qtBlokLib/parsers/mm_coord.h \
    qtBlokLib/parsers/mm_alt_long.h \
    qtBlokLib/parsers/mco_state.h \
    qtBlokLib/parsers/mco_mode.h \
    qtBlokLib/parsers/mco_limits.h \
    qtBlokLib/parsers/ipd_state.h \
    qtBlokLib/parsers/drivemode.h \
    qtBlokLib/parsers/display_state.h \
    qtBlokLib/parsers/canblokmessage.h \
    qtBlokLib/parsers/autolockmode.h \
    qtBlokLib/parsers/console_key.h

QMAKE_CXXFLAGS += -std=c++0x

FORMS += form.ui

#For Monitor 5 use: MONITOR_5, LIB_CAN_NICK
#For Linux use: LIB_SOCKET_CAN
