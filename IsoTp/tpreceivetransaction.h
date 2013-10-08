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
    void setDescriptors(int transmitDescriptor, int acknowlegmentDescriptor);

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
    void transactionReaceived(const std::vector<byte> &data);
    void sendFlowControl(const FlowControlFrame &frame);

public slots:
    void getSingleFrame(const SingleFrame &frame);
    void getFirstFrame(const FirstFrame &frame);
    void getConsecutiveFrame(const ConsecutiveFrame &frame);
    void timeout();
};

}


#endif // TPRECEIVETRANSACTION_H
