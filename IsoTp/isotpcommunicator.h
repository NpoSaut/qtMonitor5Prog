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
    explicit IsoTpCommunicator(int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent = 0);
    void send(const std::vector<byte> &buffer);

private:
    /*static*/ TpReceiveTransaction receiver;
    TpSendTransaction sender;

signals:
    void bufferReceived(const std::vector<byte> &buffer);

public slots:
    void transactionReceived(const std::vector<byte> &buffer);
};
}
#endif // ISOTPCOMMUNICATOR_H
