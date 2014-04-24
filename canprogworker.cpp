#include "canprogworker.h"

#include <QFileInfo>

#include "processmanager.h"
#include "ExclusiveCanUpdateManager.h"
#include "SharedCanUpdateManager.h"
#include "PropStore/FilePropStore.h"
#include "PropStore/ConstLoaderStore.h"
#include "PropStore/PropStoreDecorators.h"
#include "PropStore/CompositePropStore.h"

#ifdef LIB_CAN_NICK
#include "qtCanLib/CanNick/workingwithmessage.h"
#endif

using namespace Fudp;

CanProgWorker::CanProgWorker(Can *can, QString firmwareRootDirName, QString hwStoreFileName, QString storeFileName, QObject *parent) :
    QThread (parent),
    can (can)
{
    QFileInfo firmwareRootDirInfo (firmwareRootDirName);
    this->firmwareRootDirName = firmwareRootDirInfo.absoluteFilePath ();

    QFileInfo hwStoreFileInfo (hwStoreFileName);
    this->hwStoreFileName = hwStoreFileInfo.absoluteFilePath ();

    QFileInfo storeFileInfo (storeFileName);
    this->pStoreFileName = storeFileInfo.absoluteFilePath ();

    this->start();
}

void CanProgWorker::run()
{
    // Хранилища
    QList<PropStore *> stores;
    stores += new ConstLoaderStore (5, 2, 8, 4);
    QFile pStoreFile (pStoreFileName);
    stores += new PropStoreWithKeyRange (new FilePropStore (pStoreFile), 0, 127);
    QFile hwStoreFile (hwStoreFileName);
    stores += new PropStoreWithKeyRange (new FilePropStore (hwStoreFile), 128, 191);
    CompositePropStore storeList (stores);

    CanProg prog (can, &storeList, QDir(firmwareRootDirName));

    ProcessManager *processManager = new ProcessManager (firmwareRootDirName, false);
    UpdateManager *updateManager;
#ifdef LIB_CAN_NICK
    updateManager = new ExclusiveCanUpdateManager (processManager);
    QObject::connect ((ExclusiveCanUpdateManager *)processManager, SIGNAL(startDriverRequest()), &canDrv, SLOT(start()));
    QObject::connect ((ExclusiveCanUpdateManager *)processManager, SIGNAL(stopDriverRequest()), &canDrv, SLOT(stop()));
#else
    updateManager = new SharedCanUpdateManager (processManager);
#endif

    QObject::connect(&prog, SIGNAL(sendState(QString)), this, SLOT(processProgStateChange(QString)));
    QObject::connect(&prog, SIGNAL(noSerialNumber()), this, SLOT(processProgSerialNumberMissedSignal()));
    QObject::connect (&prog, SIGNAL(progModeChanged(bool)), this, SLOT(processProgModeChange(bool)));
    QObject::connect(this, SIGNAL(serialNumberChanged(qint32)), &prog, SLOT(inputBlockSerialNumber(qint32)));

    QObject::connect (&prog, SIGNAL(progModeChanged(bool)), updateManager, SLOT(setUpdateMode(bool)));
    QObject::connect (&prog, SIGNAL(crcCheckChanged(bool)), updateManager, SLOT(setIsChecksumOk(bool)));

    updateManager->execute ();
    prog.checkFirmware ();

    QThread::run ();
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
