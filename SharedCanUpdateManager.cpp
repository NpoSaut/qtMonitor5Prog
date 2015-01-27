#include "SharedCanUpdateManager.h"

SharedCanUpdateManager::SharedCanUpdateManager(ProcessManager *process, QObject *parent) :
    UpdateManager(process, parent)
{
}

void SharedCanUpdateManager::applyNewState()
{
    if ( !updateMode && crcOk)
        process->start();
    else
        process->stop();
}
