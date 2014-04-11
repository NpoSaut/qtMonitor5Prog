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
#include "progwriteack.h"
#include "paramsetack.h"
#include "paramsetrq.h"
#include "paramrmack.h"
#include "paramrmrq.h"
#include "progbroadcastanswer.h"
#include "progfirmcorrupt.h"
#include "progsubmit.h"
#include "progsubmitack.h"
#include "progping.h"
#include "progpong.h"

using namespace IsoTp;

namespace FudpMessage
{
class WorkingWithFudpMessage : public QObject
{
    Q_OBJECT
public:
    explicit WorkingWithFudpMessage(Can *can, int initDescriptor, int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent = 0);

private:
    IsoTpCommunicator communicator1;
    IsoTpCommunicator communicator2;
    int count;

signals:
    void transmitData(std::vector<byte> data);

    void getProgInit(DeviceTicket tickets);
    void getProgListRq();
    void getProgReadRq(QString fileName, qint32 offset, qint32 readSize);
    void getProgRm(QString fileName);
    void getProgMrPropper(qint32 securityKey);
    void getProgCreate(QString fileName, qint32 fileSize);
    void getProgWrite(QString fileName, qint32 offset, QByteArray data);
    void getParamSetRq(qint8 key, qint32 value);
    void getParamRmRq(qint8 key);
    void getProgSubmit(qint8 key);
    void getProgPing(quint8 counter);

    void waitingTimeOut();

public slots:
    void receiveData(const std::vector<byte> &data);

    void sendAnswerToBroadcast(DeviceTicket ticket);
    void sendProgStatus(const QVector<QPair<quint8, qint32> > dictionary);
    void sendProgList(QMap<QString, DevFileInfo> list);
    void sendProgRead(qint8 errorCode, const QByteArray &data);
    void sendProgRmAck(qint8 errorCode);
    void sendProgMrPropperAck();
    void sendProgCreateAck(qint8 errorCode);
    void sendProgWriteAck(qint8 errorCode);
    void sendParamSetAck(qint8 errorCode);
    void sendParamRmAck(qint8 errorCode);
    void sendProgFirmCorrupt();
    void sendSubmitAck(qint8 finalCode);
    void sendProgPong(quint8 counter, ProgPong::Status state);

    void timeOut();
};
}

#endif // WORKINGWITHFUDPMESSAGE_H
