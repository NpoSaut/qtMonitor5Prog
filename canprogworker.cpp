#include "canprogworker.h"

using namespace Fudp;

CanProgWorker::CanProgWorker(QString storeFileName, QObject *parent) :
    QThread(parent),
    storeFileName (storeFileName)
{
    this->start();
}

void CanProgWorker::run()
{
    QFile storeFile (storeFileName);
    pStore = new SimpleFilePropStore (storeFile);
    prog = new CanProg (pStore);

    QObject::connect(prog, SIGNAL(sendState(QString)), this, SLOT(processProgStateChange(QString)));
    QObject::connect(prog, SIGNAL(noSerialNumber()), this, SLOT(processProgSerialNumberMissedSignal()));
    QObject::connect(prog, SIGNAL(exit()), this, SLOT(processProgExit()));
    QObject::connect(prog, SIGNAL(initConnection()), this, SLOT(processProgConnect()));

    prog->drvStart();
    QThread::run();
}

void CanProgWorker::storeSerialNumber (qint32 sn)
{
    prog->inputBlockSerialNumber(sn);
}

void CanProgWorker::processProgStateChange(QString s)
{
    emit stateChanged(s);
}

void CanProgWorker::processProgSerialNumberMissedSignal()
{
    emit serialNumberMissed();
}

void CanProgWorker::processProgExit()
{
    emit inProgModeChanged(false);
}

void CanProgWorker::processProgConnect()
{
    emit inProgModeChanged(true);
}
