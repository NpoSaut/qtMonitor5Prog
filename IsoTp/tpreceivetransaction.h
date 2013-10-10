#ifndef TPRECEIVETRANSACTION_H
#define TPRECEIVETRANSACTION_H

#include <QObject>
#include <QTimer>
#include "IsoTpFrame/workingwithframes.h"

using namespace IsoTpFrames;

namespace IsoTp
{
class TpReceiveTransaction : public QObject
{
    Q_OBJECT
public:
    explicit TpReceiveTransaction(/*int transmitDescriptor, int acknowlegmentDescriptor, */QObject *parent = 0);
    void setTransmitDescriptor(int transmitDescriptor);
    void setAcknowlegmentDescriptor(int acknowlegmentDescriptor);

private:
    WorkingWithFrames movingFrames;
    QTimer timer;
    std::vector<byte> buffer;
    std::vector<byte>::iterator pointer;
    uint buffLength;
    int blockSize;
    int consIndex;

    void readyFlowControl();

signals:
    void transactionReaceived(std::vector<byte> data);
    void sendFlowControl(FlowControlFrame frame);

public slots:
    void getSingleFrame(SingleFrame frame);
    void getFirstFrame(FirstFrame frame);
    void getConsecutiveFrame(ConsecutiveFrame frame);
    void timeout();
};

}


#endif // TPRECEIVETRANSACTION_H
