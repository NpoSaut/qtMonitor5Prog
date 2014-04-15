#ifndef ISOTPCOMMUNICATOR_H
#define ISOTPCOMMUNICATOR_H

#include <QObject>
#include "tpreceivetransaction.h"
#include "tpsendtransaction.h"

namespace IsoTp
{
class IsoTpCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit IsoTpCommunicator(Can *can, int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent = 0);
    void setAcknowlegmentDescriptor(int acknowlegmentDescriptor);

    void send(const std::vector<byte> &buffer);

signals:
    void bufferReceived(const std::vector<byte> &buffer);
    void waitingTimeOut();

private:
    TpReceiveTransaction receiver;
    TpSendTransaction sender;

private slots:
    void onTransactionReceived(const std::vector<byte> &buffer);
    void timeOut();
};
}
#endif // ISOTPCOMMUNICATOR_H
