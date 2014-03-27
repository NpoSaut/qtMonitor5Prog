#include <QMetaType>

#include "workingwithframes.h"
#include "Log/logwriter.h"

namespace IsoTpFrames
{

WorkingWithFrames::WorkingWithFrames(Can *can, QObject *parent) :
    QObject(parent),
    can (can)
{
    qRegisterMetaType<CanFrame>("SingleFrame");
    qRegisterMetaType<CanFrame>("FistFrame");
    qRegisterMetaType<CanFrame>("ConsecutiveFrame");
    qRegisterMetaType<CanFrame>("FlowControlFrame");

    QObject::connect(this, SIGNAL(transmitCanFrame(CanFrame)), can, SLOT(transmitMessage(CanFrame)));
    QObject::connect(can, SIGNAL(messageReceived(CanFrame)), this, SLOT(receiveCanFrame(CanFrame)));
}

void WorkingWithFrames::receiveCanFrame(const CanFrame &frame)
{
    if(acknowlegmentDescriptor == frame.getDescriptor())
        switch ((frame[1] & 0xf0) >> 4)
        {
        case IsoTpFrameType(singleFrame):
        {
            std::vector<byte> v(frame.getData().begin()+1, frame.getData().end());
            SingleFrame sFrame(v);
            emit receiveSingleFrame(sFrame);
            break;
        }
        case IsoTpFrameType(firstFrame):
        {
            std::vector<byte> v(frame.getData().begin()+2, frame.getData().end());
            FirstFrame fFrame(v, ((frame[1] & 0x0f) << 8) | frame[2] );
            emit receiveFirstFrame(fFrame);
            break;
        }
        case IsoTpFrameType(consecutiveFrame):
        {
            std::vector<byte> v(frame.getData().begin()+1, frame.getData().end());
            ConsecutiveFrame cFrame(v,frame[1] & 0x0f);
            emit receiveConsecutiveFrame(cFrame);
            break;
        }
        case IsoTpFrameType(flowControlFrame):
        {
            FlowControlFrame fcFrame((FlowControlFlag)(frame[1]&0x0f),frame[2],frame[3]);
            emit receiveFlowControlFrame(fcFrame);
            break;
        }
        default:
        {
            //qDebug() << "Invalid code";
            break;
        }
        }

}

void WorkingWithFrames::transmitSingleFrame(SingleFrame frame)
{
    CanFrame cFrame = frame.getCanFrame();
    cFrame.setDescriptor(transmitDescriptor);
    emit transmitCanFrame(cFrame);
}

void WorkingWithFrames::transmitFirstFrame(FirstFrame frame)
{
    CanFrame cFrame = frame.getCanFrame();
    cFrame.setDescriptor(transmitDescriptor);
    emit transmitCanFrame(cFrame);
}

void WorkingWithFrames::transmitConsecutiveFrame(ConsecutiveFrame frame)
{
    CanFrame cFrame = frame.getCanFrame();
    cFrame.setDescriptor(transmitDescriptor);
    emit transmitCanFrame(cFrame);
}

void WorkingWithFrames::transmitFlowControlFrame(FlowControlFrame frame)
{
    CanFrame cFrame = frame.getCanFrame();
    cFrame.setDescriptor(transmitDescriptor);
    emit transmitCanFrame(cFrame);
}

void WorkingWithFrames::setAcknowlegmentDescriptor(int ad)
{
    acknowlegmentDescriptor = ad;
}

int WorkingWithFrames::getAcknowlegmentDescriptor()
{
    return acknowlegmentDescriptor;
}

void WorkingWithFrames::setTransmitDescriptor(int td)
{
    transmitDescriptor = td;
}

int WorkingWithFrames::getTransmitDescriptor()
{
    return transmitDescriptor;
}
}
