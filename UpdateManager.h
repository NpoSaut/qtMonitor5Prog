#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QObject>
#include "processmanager.h"

class UpdateManager : public QObject
{
    Q_OBJECT
public:
    explicit UpdateManager(ProcessManager *process, QObject *parent = 0);

public slots:
    virtual void setUpdateMode (bool updateMode);
    virtual void setIsChecksumOk (bool ok);
    virtual void execute ();

protected:
    virtual void applyNewState () = 0;

    ProcessManager *process;
    bool updateMode;
    bool crcOk;
};

#endif // UPDATEMANAGER_H
