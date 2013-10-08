#include "isotpcommunicator.h"
namespace IsoTp
{
//TpReceiveTransaction IsoTpCommunicator::receiver;

IsoTpCommunicator::IsoTpCommunicator(int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent) :
    sender(transmitDescriptor, acknowlegmentDescriptor), QObject(parent)
{

    receiver.setDescriptors(transmitDescriptor, acknowlegmentDescriptor);

    QObject::connect(&receiver, SIGNAL(transactionReaceived(std::vector<byte>)), this, SLOT(transactionReceived(std::vector<byte>)));

}

void IsoTpCommunicator::send(const std::vector<byte> &buffer)
{
    sender.send(buffer);
}

void IsoTpCommunicator::transactionReceived(const std::vector<byte> &buffer)
{
    emit bufferReceived(buffer);
}
}
