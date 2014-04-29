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

CanProgWorker::CanProgWorker(Can *can, Parser *parser, QString firmwareRootDirName, QString hwStoreFileName, QString storeFileName, QObject *parent) :
    QThread (parent),
    can (can),
    parser (parser),
    auxResourceAnswer (nullptr),
    propStore (nullptr)
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
    propStore = new CompositePropStore (stores);

    CanProg prog (can, propStore, QDir(firmwareRootDirName));

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
    if ( inProgMode )
        stopAuxResourceAnswer ();
    else
        startAuxResourceAnswer ();

    emit inProgModeChanged (inProgMode);
}

void CanProgWorker::startAuxResourceAnswer()
{
    if ( !auxResourceAnswer )
    {
        qint32 auxModuleNumber = 0;
        qint32 auxDescriptor = 0;
        qint32 version = 0;
        qint32 subversion = 0;
        qint32 checksum = 0;
        if (    propStore->get (135, auxModuleNumber)
             && propStore->get (136, auxDescriptor)
             && propStore->get (  1, version)
             && propStore->get (  2, subversion)
             && propStore->get (  6, checksum)
            )
        {
            qDebug() << "create";
            auxResourceAnswer = new AuxResourceAnswer (can, parser, auxModuleNumber, auxDescriptor, version, subversion, checksum);
        }
    }
}

void CanProgWorker::stopAuxResourceAnswer()
{
    if ( auxResourceAnswer )
    {
        auxResourceAnswer->deleteLater ();
        auxResourceAnswer = nullptr;
    }
}
