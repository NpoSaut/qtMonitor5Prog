#ifndef EXCLUSIVECANUPDATEMANAGER_H
#define EXCLUSIVECANUPDATEMANAGER_H

#include "UpdateManager.h"

#include <QTimer>

class ExclusiveCanUpdateManager : public UpdateManager
{
    Q_OBJECT
public:
    explicit ExclusiveCanUpdateManager(ProcessManager *process, QObject *parent = 0);

signals:
    void startDriverRequest ();
    void stopDriverRequest ();

private:
    void applyNewState();
    void startProgram();
    void stopProgram();

    QTimer startDelayTimer;

private slots:
    void onStartDelayTimerOut ();
};

#endif // EXCLUSIVECANUPDATEMANAGER_H
