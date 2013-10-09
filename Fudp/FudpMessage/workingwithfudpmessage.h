#ifndef WORKINGWITHFUDPMESSAGE_H
#define WORKINGWITHFUDPMESSAGE_H

#include <QObject>
#include "../../IsoTp/isotpcommunicator.h"
#include "proginit.h"
#include "progstatus.h"
#include "progcreate.h"
#include "progcreateack.h"
#include "proglist.h"
#include "proglistrq.h"
#include "progmrproper.h"
#include "progmrproperack.h"
#include "progread.h"
#include "progreadrq.h"
#include "progrm.h"
#include "progrmack.h"
#include "progwrite.h"
#include "paramsetack.h"
#include "paramsetrq.h"
#include "paramrmack.h"
#include "paramrmrq.h"
#include "QHash"

using namespace IsoTp;

namespace FudpMessage
{
struct DeviceTickets
{
    qint8 systemId;
    qint16 blockId;
    qint8 blockModification;

    bool operator == (const DeviceTickets &ticket);
};

class WorkingWithFudpMessage : public QObject
{
    Q_OBJECT
public:
    explicit WorkingWithFudpMessage(QObject *parent = 0);

private:
    IsoTpCommunicator communicator;


signals:
    void transmitData(std::vector<byte> data);

    void getProgInit(DeviceTickets tickets);
    void getProgListRq();
    void getProgReadRq(QString fileName, qint32 offset, qint32 readSize);
    void getProgRm(QString fileName);
    void getProgMrPropper(qint32 securityKey);
    void getProgCreate(QString fileName, qint32 fileSize);
    void getProgWrite(QString fileName, qint32 offset, QByteArray data);
    void getParamSetRq(qint8 key, qint32 value);
    void getParamRmRq(qint8 key);

public slots:
    void receiveData(const std::vector<byte> &data);

    void sendProgStatus(const QHash<qint8, qint32> &dictionary);
    void sendProgList(const QList<DevFileInfo> &list);
    void sendProgRead(qint8 errorCode, const QByteArray &data);
    void sendProgRmAck(qint8 errorCode);
    void sendProgMrPropperAck();
    void sendProgCreateAck(qint8 errorCode);
    void sendParamSetAck(qint8 errorCode);
    void sendParamRmAck(qint8 errorCode);
};
}

#endif // WORKINGWITHFUDPMESSAGE_H
