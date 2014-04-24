#ifndef CANPROGWORKER_H
#define CANPROGWORKER_H

#include <QString>
#include <QThread>

#include "Fudp/canprog.h"
#include "PropStore/FilePropStore.h"

class CanProgWorker : public QThread
{
    Q_OBJECT
public:
    explicit CanProgWorker(Can *can, QString firmwareRootDirName, QString hwStoreFileName, QString pStoreFileName, QObject *parent = 0);
    void run();

signals:
    void stateChanged (QString);
    void serialNumberMissed ();
    void inProgModeChanged (bool);
    void progModeEnter ();
    void progModeExit ();

    //private:
    void serialNumberChanged (qint32 sn);
    
public slots:
    void storeSerialNumber (qint32 sn);

private slots:
    void processProgStateChange (QString s);
    void processProgSerialNumberMissedSignal ();
    void processProgModeChange (bool inProgMode);
private:
    Can *can;
    QString hwStoreFileName;
    QString pStoreFileName;
    QString firmwareRootDirName;
};

#endif // CANPROGWORKER_H
