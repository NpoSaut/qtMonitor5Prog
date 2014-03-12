#ifndef CANPROGWORKER_H
#define CANPROGWORKER_H

#include <QString>
#include <QThread>

#include "Fudp/canprog.h"
#include "PropStore/simplefilepropstore.h"

class CanProgWorker : public QThread
{
    Q_OBJECT
public:
    explicit CanProgWorker(Can *can, QString storeFileName, QObject *parent = 0);
    void run();

signals:
    void stateChanged (QString);
    void serialNumberMissed ();
    void inProgModeChanged (bool);
    //private:
    void serialNumberChanged (qint32 sn);
    
public slots:
    void storeSerialNumber (qint32 sn);

private slots:
    void processProgStateChange (QString s);
    void processProgSerialNumberMissedSignal ();
    void processProgExit ();
    void processProgConnect ();

private:
    const Can *can;
    QString storeFileName;
};

#endif // CANPROGWORKER_H
