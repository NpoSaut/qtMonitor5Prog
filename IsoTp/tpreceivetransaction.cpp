#include "tpreceivetransaction.h"
#include "Log/logwriter.h"

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
    timer.stop();
    emit transactionReaceived(frame.getData());
}

void TpReceiveTransaction::getFirstFrame(FirstFrame frame)
{
    if (state == INIT || state == BROKEN)
    {
        timer.stop();
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
        LOG_WRITER.write(tr("FirstFrame пришел невовремя"), QColor(255, 0, 255), 1);
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
            else
                timer.start(3000);
            if ((buffer.size() == buffLength))
            {
                timer.stop();
                state = INIT;
                emit transactionReaceived(buffer);
            }
        }
        else
        {
            state = BROKEN;
            sendAbort();
            LOG_WRITER.write(QString(tr("IsoTp последовательность нарушена. Ожидался фрейм с индексом %1, а получен - с индексом %2")).
                             arg(consecutiveFrameCounter & 0x0F).
                             arg(frame.getIndex()), QColor(255, 0, 255), 1);
//            qDebug() << "IsoTp sequence fail. Wait "
//                     << (consecutiveFrameCounter & 0x0F) << ", but got " << frame.getIndex() << ". :(";
        }
    }
    else if (state == INIT)
    {
        LOG_WRITER.write(tr("ConsecutiveFrame пришел невовремя"), QColor(255, 0, 0), 1);
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
    sendAbort();
    LOG_WRITER.write(tr("Вышло время ожидания сообщения"), QColor(255, 0, 0), 1);
    emit watingTimeOut();
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
