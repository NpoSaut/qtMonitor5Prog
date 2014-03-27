#include <QDebug>

#include "tpsendtransaction.h"

#include <QDebug>

namespace IsoTp
{
TpSendTransaction::TpSendTransaction(Can *can, int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent) :
    QObject(parent),
    movingFrames (can, parent)
{

    separationTime = 0;

    movingFrames.setTransmitDescriptor(transmitDescriptor);
    movingFrames.setAcknowlegmentDescriptor(acknowlegmentDescriptor);

    QObject::connect(this, SIGNAL(sendSingleFrame(SingleFrame)), &movingFrames, SLOT(transmitSingleFrame(SingleFrame)));
    QObject::connect(this, SIGNAL(sendFirstFrame(FirstFrame)), &movingFrames, SLOT(transmitFirstFrame(FirstFrame)));
    QObject::connect(this, SIGNAL(sendConsecutiveFrame(ConsecutiveFrame)), &movingFrames, SLOT(transmitConsecutiveFrame(ConsecutiveFrame)));

    QObject::connect(&movingFrames, SIGNAL(receiveFlowControlFrame(FlowControlFrame)), this, SLOT(getFlowControl(FlowControlFrame)));
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(sendConsecutive()));
}

void TpSendTransaction::send(const std::vector<byte> &buffer)
{
    buff.clear();
    if (buffer.size() <= 7)
    {
        SingleFrame sFrame(buffer);
        emit sendSingleFrame(sFrame);
    }
    else
        if (buffer.size() > 4095)
            qDebug() << "Too large buffer";
        else
        {
            blockSize = 0;
            consecutiveFrameIndex = 1;
            consecutiveFramesSent = 1;
            buff.insert(buff.begin(), buffer.begin()+6, buffer.end());
            pointer = buff.begin();
            std::vector<byte> v;
            v.insert(v.begin(), buffer.begin(), buffer.begin() + 6);
            FirstFrame fFrame(v, buffer.size());
            emit sendFirstFrame(fFrame);
        }
}

void TpSendTransaction::getFlowControl(FlowControlFrame frame)
{
    if (frame.getFrlag() == FlowControlFlag(Abort))
        timer.stop();
    else
        if (frame.getFrlag() == FlowControlFlag(ClearToSend))
        {
            consecutiveFrameIndex = 1;
            separationTime = frame.getSeparationTime();
            blockSize = frame.getBlockSize();
            timer.start(separationTime);
        }
}

void TpSendTransaction::sendConsecutive()
{
    if ((consecutiveFrameIndex == blockSize+1) || (std::distance(buff.begin(), pointer) >= buff.size()))
    {
        timer.stop();
    }
    else
    {
        //qDebug("Index: %d, blockSize: %d", consecutiveFrameIndex,blockSize);
        std::vector<byte> v;
        int dataLength  = (buff.size() - std::distance(buff.begin(), pointer)) > 7 ? 7 : (buff.size() - std::distance(buff.begin(), pointer));
        v.insert(v.begin(), pointer, pointer + dataLength);
        ConsecutiveFrame cFrame(v, consecutiveFramesSent);
        emit sendConsecutiveFrame(cFrame);
        consecutiveFrameIndex++;
        consecutiveFramesSent ++;
        pointer += dataLength;
    }
}
void TpSendTransaction::setAcknowlegmentDescriptor(int acknowlegmentDescriptor)
{
    movingFrames.setAcknowlegmentDescriptor(acknowlegmentDescriptor);
}
}
