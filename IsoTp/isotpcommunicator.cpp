#include "isotpcommunicator.h"
#include "Log/logwriter.h"
namespace IsoTp
{

IsoTpCommunicator::IsoTpCommunicator(Can *can, int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent) :
    QObject(parent),
    sender(can, transmitDescriptor, acknowlegmentDescriptor, parent),
    receiver(can, parent)
{

    receiver.setTransmitDescriptor(transmitDescriptor);
    receiver.setAcknowlegmentDescriptor(acknowlegmentDescriptor);

    QObject::connect(&receiver, SIGNAL(transactionReaceived(std::vector<byte>)), this, SLOT(transactionReceived(std::vector<byte>)));
    QObject::connect(&receiver, SIGNAL(watingTimeOut()), this, SLOT(timeOut()));
    QObject::connect(this, SIGNAL(startFlowControl(bool)), &receiver, SLOT(onActionMode(bool)));

}

void IsoTpCommunicator::send(const std::vector<byte> &buffer)
{
    sender.send(buffer);
}

void IsoTpCommunicator::transactionReceived(const std::vector<byte> &buffer)
{
    emit bufferReceived(buffer);
}

void IsoTpCommunicator::setAcknowlegmentDescriptor(int acknowlegmentDescriptor)
{
    receiver.setAcknowlegmentDescriptor(acknowlegmentDescriptor);
    sender.setAcknowlegmentDescriptor(acknowlegmentDescriptor);
}

void IsoTpCommunicator::timeOut()
{
    emit waitingTimeOut();
}

void IsoTpCommunicator::startReceiver(bool start)
{
    emit startFlowControl(start);
}
}
