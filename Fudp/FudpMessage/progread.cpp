#include "progread.h"

namespace FudpMessage
{
ProgRead::ProgRead()
{
}

ProgRead::ProgRead(const QByteArray &fData, qint8 eCode) :
    data(fData), errorCode(eCode)
{
}

std::vector<byte> ProgRead::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    if (errorCode != 0)
        in << (byte)MessageId(progRead) << (byte)errorCode;
    else
        in << (byte)MessageId(progRead) << (byte)errorCode << data;
    return Message::fromQByteArrayToVector(buffer);
}

void ProgRead::decode(const std::vector<byte> &data)
{
    errorCode = data.at(1);
    if(errorCode == 0)
    {
        QByteArray buffer = Message::fromVectorToQByteArray(data);
        QDataStream out(&buffer, QIODevice::ReadOnly);
        out.setByteOrder(QDataStream::LittleEndian);
        out.skipRawData(1);
        out >> this->data;
    }
}

QByteArray ProgRead::getData()
{
    return data;
}

qint8 ProgRead::getErrorCode()
{
    return errorCode;
}
}
