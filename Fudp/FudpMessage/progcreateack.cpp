#include "progcreateack.h"

namespace FudpMessage
{
ProgCreateAck::ProgCreateAck()
{
    createErrors[1] = "File with the same name already exists.";
    createErrors[2] = "Exceeded the maximum numbers of files.";
    createErrors[3] = "Out of memory.";
    createErrors[4] = "Error creating";
}

ProgCreateAck::ProgCreateAck(qint8 errCode) :
    errorCode(errCode)
{
}

std::vector<byte> ProgCreateAck::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in << MessageId(progCreateAck) << errorCode;
}

void ProgCreateAck::decode(const std::vector<byte> &data)
{
    errorCode = data.at(1);
}
}
