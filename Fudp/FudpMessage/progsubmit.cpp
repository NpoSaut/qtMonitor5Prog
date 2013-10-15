#include "progsubmit.h"

using namespace FudpMessage;

ProgSubmit::ProgSubmit()
{
}


std::vector<byte> FudpMessage::ProgSubmit::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in << MessageId(progSubmit);

    return Message::fromQByteArrayToVector(buffer);
}

void FudpMessage::ProgSubmit::decode(const std::vector<byte> &data)
{
}
