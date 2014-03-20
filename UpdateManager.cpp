#include "UpdateManager.h"

UpdateManager::UpdateManager(ProcessManager *process, QObject *parent) :
    QObject(parent),
    process (process),
    updateMode (false),
    crcOk (false)
{
}

void UpdateManager::setUpdateMode(bool updateMode)
{
    if (this->updateMode != updateMode)
    {
        this->updateMode = updateMode;
        applyNewState ();
    }
}

void UpdateManager::setIsChecksumOk(bool ok)
{
    if (crcOk != ok)
    {
        crcOk = ok;
        applyNewState ();
    }
}

void UpdateManager::execute()
{
    applyNewState ();
}
