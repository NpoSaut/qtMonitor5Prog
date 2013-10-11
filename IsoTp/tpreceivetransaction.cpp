#include "tpreceivetransaction.h"
namespace IsoTp
{
TpReceiveTransaction::TpReceiveTransaction(/*int transmitDescriptor, int acknowlegmentDescriptor, */QObject *parent) :
    QObject(parent)
{
    blockSize = 20;
    consIndex = blockSize;    

    QObject::connect(&movingFrames, SIGNAL(receiveSingleFrame(SingleFrame)), this, SLOT(getSingleFrame(SingleFrame)));
    QObject::connect(&movingFrames, SIGNAL(receiveFirstFrame(FirstFrame)), this, SLOT(getFirstFrame(FirstFrame)));
    QObject::connect(&movingFrames, SIGNAL(receiveConsecutiveFrame(ConsecutiveFrame)), this, SLOT(getConsecutiveFrame(ConsecutiveFrame)));

    QObject::connect(this, SIGNAL(sendFlowControl(FlowControlFrame)), &movingFrames, SLOT(transmitFlowControlFrame(FlowControlFrame)));
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));

    timer.start(3000);
}

void TpReceiveTransaction::getSingleFrame(SingleFrame frame)
{
    emit transactionReaceived(frame.getData());
}

void TpReceiveTransaction::getFirstFrame(FirstFrame frame)
{
    buffer.clear();
    buffLength = frame.getPacketSize();
    std::vector<byte> v = frame.getData();
    pointer = buffer.begin();
    buffer.insert(pointer, v.begin(), v.end());
    pointer = buffer.end();
    readyFlowControl();
}

void TpReceiveTransaction::getConsecutiveFrame(ConsecutiveFrame frame)
{
    timer.stop();
    std::vector<byte> v = frame.getData();
    buffer.insert(pointer, v.begin(), (buffLength -  std::distance(buffer.begin(), pointer)) > 7 ?
                      v.end() : v.begin() + buffLength -  std::distance(buffer.begin(), pointer));
    pointer = buffer.end();
    consIndex++;
    if ((buffer.size() < buffLength) && (blockSize == consIndex))
        readyFlowControl();
    if ((buffer.size() == buffLength))
        emit transactionReaceived(buffer);
}

void TpReceiveTransaction::readyFlowControl()
{
    consIndex = 0;
    FlowControlFrame fcFrame(FlowControlFlag(ClearToSend), blockSize, 0);
    timer.start(3000);
    emit sendFlowControl(fcFrame);
}

void TpReceiveTransaction::timeout()
{
    timer.stop();
    //qDebug() << "The waiting time is over";
}

void TpReceiveTransaction::setTransmitDescriptor(int transmitDescriptor)
{
    movingFrames.setTransmitDescriptor(transmitDescriptor);
}

void TpReceiveTransaction::setAcknowlegmentDescriptor(int acknowlegmentDescriptor)
{
    movingFrames.setAcknowlegmentDescriptor(acknowlegmentDescriptor);
}
}
