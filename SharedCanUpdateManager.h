#ifndef SHAREDCANUPDATEMANAGER_H
#define SHAREDCANUPDATEMANAGER_H

#include "UpdateManager.h"

class SharedCanUpdateManager : public UpdateManager
{
    Q_OBJECT
public:
    explicit SharedCanUpdateManager(ProcessManager *process, QObject *parent = 0);
    
protected:
    virtual void applyNewState();
};

#endif // SHAREDCANUPDATEMANAGER_H
