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
    explicit CanProgWorker(QString storeFileName, QObject *parent = 0);
    void run();

signals:
    void stateChanged (QString);
    void serialNumberMissed ();
    void inProgModeChanged (bool);
    
public slots:
    void storeSerialNumber (qint32 sn);

private slots:
    void processProgStateChange (QString s);
    void processProgSerialNumberMissedSignal ();
    void processProgExit ();
    void processProgConnect ();

private:
    SimpleFilePropStore *pStore;
    Fudp::CanProg *prog;

    QString storeFileName;
};

#endif // CANPROGWORKER_H
