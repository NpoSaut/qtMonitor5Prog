#include "progstatus.h"

namespace FudpMessage
{
ProgStatus::ProgStatus()
{
}

ProgStatus::ProgStatus(const QHash<qint8, qint32> &properties)    :
    properties(properties)
{
}

std::vector<byte> ProgStatus::encode()
{
    QByteArray arr;
    QDataStream in(&arr, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in << (byte)MessageId(progStatus);

    foreach (qint8 i, properties.keys())
    {
        in << i << properties.value(i);
    }
    return Message::fromQByteArrayToVector(arr);
}

void ProgStatus::decode(const std::vector<byte> &data)
{
    QByteArray buffer = Message::fromVectorToQByteArray(data);
    QDataStream out (&buffer, QIODevice::ReadOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out.skipRawData(1);
    while(!out.atEnd())
    {
        qint8 key;
        qint32 value;
        out >> key;
        out >> value;
        properties[key] = value;
    }
}

QHash<qint8, qint32> ProgStatus::getProperties()
{
    return properties;
}
}
