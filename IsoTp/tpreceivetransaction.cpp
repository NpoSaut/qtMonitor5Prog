#include "tpreceivetransaction.h"
namespace IsoTp
{
TpReceiveTransaction::TpReceiveTransaction(/*int transmitDescriptor, int acknowlegmentDescriptor, */QObject *parent) :
    QObject(parent), state (INIT)
{
    blockSize = 32;
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
    if (state == INIT || state == BROKEN)
    {
        state = PROGRESS;
        buffer.clear();
        buffLength = frame.getPacketSize();
        consecutiveFrameCounter = 0;
        std::vector<byte> v = frame.getData();
        pointer = buffer.begin();
        buffer.insert(pointer, v.begin(), v.end());
        pointer = buffer.end();
        readyFlowControl();
    }
    else if (state == PROGRESS)
    {
        state = BROKEN;
        sendAbort();
    }
}

void TpReceiveTransaction::getConsecutiveFrame(ConsecutiveFrame frame)
{
    if (state == PROGRESS)
    {
        timer.stop();
        std::vector<byte> v = frame.getData();
        if ( (++consecutiveFrameCounter & 0x0F) == frame.getIndex() )
        {
            buffer.insert(pointer, v.begin(), (buffLength -  std::distance(buffer.begin(), pointer)) > 7 ?
                              v.end() : v.begin() + buffLength -  std::distance(buffer.begin(), pointer));
            pointer = buffer.end();
            consIndex++;
            if ((buffer.size() < buffLength) && (blockSize == consIndex))
                readyFlowControl();
            if ((buffer.size() == buffLength))
            {
                state = INIT;
                emit transactionReaceived(buffer);
            }
        }
        else
        {
            state = BROKEN;
            sendAbort();
            qDebug() << "IsoTp sequence fail. Wait "
                     << (consecutiveFrameCounter & 0x0F) << ", but got " << frame.getIndex() << ". :(";
        }
    }
    else if (state == INIT)
    {
        state = BROKEN;
        sendAbort();
    }
    else if (state == BROKEN)
    {
        // silent
    }
}

void TpReceiveTransaction::readyFlowControl()
{
    consIndex = 0;
    FlowControlFrame fcFrame(FlowControlFlag(ClearToSend), blockSize, 0);
    timer.start(3000);
    emit sendFlowControl(fcFrame);
}

void TpReceiveTransaction::sendAbort()
{
    FlowControlFrame fcFrame(FlowControlFlag::Abort, 0, 0);
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
