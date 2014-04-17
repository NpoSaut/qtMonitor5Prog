#include "isotpcommunicator.h"
#include "Log/logwriter.h"
namespace IsoTp
{

IsoTpCommunicator::IsoTpCommunicator(Can *can, int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent) :
    QObject(parent),
    sender(can, transmitDescriptor, acknowlegmentDescriptor, this),
    receiver(can, this)
{
    receiver.setTransmitDescriptor(transmitDescriptor);
    receiver.setAcknowlegmentDescriptor(acknowlegmentDescriptor);

    QObject::connect(&receiver, SIGNAL(transactionReceived(std::vector<byte>)), this, SLOT(onTransactionReceived(std::vector<byte>)));
    QObject::connect(&receiver, SIGNAL(watingTimeOut()), this, SLOT(timeOut()));

}

void IsoTpCommunicator::send(const std::vector<byte> &buffer)
{
    sender.send(buffer);
}

void IsoTpCommunicator::onTransactionReceived(const std::vector<byte> &buffer)
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
}
