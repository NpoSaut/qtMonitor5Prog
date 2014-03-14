#include "canprogworker.h"

using namespace Fudp;

CanProgWorker::CanProgWorker(Can *can, QString storeFileName, QObject *parent) :
    QThread(parent),
    storeFileName (storeFileName),
    can (can)
{
    this->start();
}

void CanProgWorker::run()
{
    QFile storeFile (storeFileName);
    SimpleFilePropStore pStore (storeFile);
    CanProg prog (can, &pStore, this->parent ());

    QObject::connect(&prog, SIGNAL(sendState(QString)), this, SLOT(processProgStateChange(QString)));
    QObject::connect(&prog, SIGNAL(noSerialNumber()), this, SLOT(processProgSerialNumberMissedSignal()));
    QObject::connect(&prog, SIGNAL(exit()), this, SLOT(processProgExit()));
    QObject::connect(&prog, SIGNAL(initConnection()), this, SLOT(processProgConnect()));
    QObject::connect(this, SIGNAL(serialNumberChanged(qint32)), &prog, SLOT(inputBlockSerialNumber(qint32)));

    prog.drvStart();
    QThread::run();
}

void CanProgWorker::storeSerialNumber (qint32 sn)
{
    emit serialNumberChanged(sn);
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
