#include "canprogworker.h"

#include <QFileInfo>

#include "processmanager.h"
#include "ExclusiveCanUpdateManager.h"
#include "SharedCanUpdateManager.h"

#ifdef LIB_CAN_NICK
#include "qtCanLib/CanNick/workingwithmessage.h"
#endif

using namespace Fudp;

CanProgWorker::CanProgWorker(Can *can, QString firmwareRootDirName, QString storeFileName, QObject *parent) :
    QThread (parent),
    can (can)
{
    QFileInfo firmwareRootDirInfo (firmwareRootDirName);
    this->firmwareRootDirName = firmwareRootDirInfo.absoluteFilePath ();

    QFileInfo storeFileInfo (storeFileName);
    this->storeFileName = storeFileInfo.absoluteFilePath ();

    this->start();
}

void CanProgWorker::run()
{
    QFile storeFile (storeFileName);
    SimpleFilePropStore pStore (storeFile);
    CanProg prog (can, &pStore, QDir(firmwareRootDirName), this->parent ());

    ProcessManager *processManager = new ProcessManager (firmwareRootDirName, this->parent ());
    UpdateManager *updateManager;
#ifdef LIB_CAN_NICK
    updateManager = new ExclusiveCanUpdateManager (processManager, this->parent ());
    QObject::connect ((ExclusiveCanUpdateManager *)processManager, SIGNAL(startDriverRequest()), &canDrv, SLOT(start()));
    QObject::connect ((ExclusiveCanUpdateManager *)processManager, SIGNAL(stopDriverRequest()), &canDrv, SLOT(stop()));
#else
    updateManager = new SharedCanUpdateManager (processManager, this->parent ());
#endif

    QObject::connect(&prog, SIGNAL(sendState(QString)), this, SLOT(processProgStateChange(QString)));
    QObject::connect(&prog, SIGNAL(noSerialNumber()), this, SLOT(processProgSerialNumberMissedSignal()));
    QObject::connect (&prog, SIGNAL(progModeChanged(bool)), this, SLOT(processProgModeChange(bool)));
    QObject::connect(this, SIGNAL(serialNumberChanged(qint32)), &prog, SLOT(inputBlockSerialNumber(qint32)));

    QObject::connect (&prog, SIGNAL(progModeChanged(bool)), updateManager, SLOT(setUpdateMode(bool)));
    QObject::connect (&prog, SIGNAL(crcCheckChanged(bool)), updateManager, SLOT(setIsChecksumOk(bool)));

    updateManager->execute ();

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

void CanProgWorker::processProgModeChange(bool inProgMode)
{
    emit inProgModeChanged (inProgMode);
}
