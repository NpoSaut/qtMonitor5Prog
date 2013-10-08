#ifndef TPSENDTRANSACTION_H
#define TPSENDTRANSACTION_H

#include <QObject>
#include <QTimer>
#include "IsoTpFrame/workingwithframes.h"

using namespace IsoTpFrames;

namespace IsoTp
{
class TpSendTransaction : public QObject
{
    Q_OBJECT
public:
    explicit TpSendTransaction(int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent = 0);
    void send(const std::vector<byte> &buffer);


private:
    int transmitDescriptor;
    int acknowlegmentDescriptor;
    int separationTime;
    int blockSize;


private:
    std::vector<byte> buff;
    std::vector<byte>::iterator pointer;
    QTimer timer;
    int consecutiveFrameSent;

    WorkingWithFrames movingFrames;

signals:
    void sendSingleFrame(SingleFrame frame);
    void sendFirstFrame(FirstFrame frame);
    void sendConsecutiveFrame(ConsecutiveFrame frame);

public slots:

    void getFlowControl(FlowControlFrame frame);
    void sendConsecutive();
};
}

#endif // TPSENDTRANSACTION_H
