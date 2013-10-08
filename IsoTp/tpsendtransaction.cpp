#include "tpsendtransaction.h"

namespace IsoTp
{
TpSendTransaction::TpSendTransaction(int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent) :
    QObject(parent)
{
    blockSize = 0;
    separationTime = 0;
    consecutiveFrameSent = 1;
    buff.clear();

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
    {
        timer.stop();
        qDebug() << "Came flag cancel the transaction";
    }
    else
        if (frame.getFrlag() == FlowControlFlag(ClearToSend))
        {
            separationTime = frame.getSeparationTime();
            blockSize += frame.getBlockSize()+1;
            timer.start(separationTime);
        }
}

void TpSendTransaction::sendConsecutive()
{
    if ((consecutiveFrameSent == blockSize) || (std::distance(buff.begin(), pointer) >= buff.size()))
    {
        timer.stop();
    }
    else
    {
        std::vector<byte> v;
        int dataLength  = (buff.size() - std::distance(buff.begin(), pointer)) > 7 ? 7 : buff.size();
        v.insert(v.begin(), pointer, pointer + dataLength);
        ConsecutiveFrame cFrame(v, consecutiveFrameSent);
        emit sendConsecutiveFrame(cFrame);
        consecutiveFrameSent++;
        pointer += dataLength;
    }
}

}
