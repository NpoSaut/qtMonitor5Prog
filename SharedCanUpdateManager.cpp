#include "SharedCanUpdateManager.h"

SharedCanUpdateManager::SharedCanUpdateManager(ProcessManager *process, QObject *parent) :
    UpdateManager(process, parent)
{
}

void SharedCanUpdateManager::applyNewState()
{
    process->keepRunning ( !updateMode && crcOk );
}
