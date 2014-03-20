#include "ExclusiveCanUpdateManager.h"

ExclusiveCanUpdateManager::ExclusiveCanUpdateManager(ProcessManager *process, QObject *parent) :
    UpdateManager(process, parent),
    startDelayTimer()
{
    startDelayTimer.setSingleShot (true);
    startDelayTimer.setInterval (2000);
    QObject::connect (&startDelayTimer, SIGNAL(timeout()), this, SLOT(onStartDelayTimerOut()));
}

void ExclusiveCanUpdateManager::applyNewState()
{
    if ( updateMode )
        stopProgram ();
    else
    {
        if ( crcOk )
            startDelayTimer.start ();
        else
            stopProgram ();
    }
}

void ExclusiveCanUpdateManager::startProgram()
{
    emit startDriverRequest ();
    process->keepRunning (true);
}

void ExclusiveCanUpdateManager::stopProgram()
{
    emit stopDriverRequest ();
    startDelayTimer.stop ();
    process->keepRunning (false);
}

void ExclusiveCanUpdateManager::onStartDelayTimerOut()
{
    startProgram ();
}
