#include "progstatus.h"

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
    //std::vector<byte> buffer;
    //buffer.insert(buffer.end(), MessageId(progStatus));
    in << (byte)MessageId(progStatus);

    foreach (qint8 i, properties.keys())
    {
        in << i << properties.value(i);
//        buffer.insert(buffer.end(), i);
//        buffer.insert(buffer.end(), (properties.value(i) & 0xff));
//        buffer.insert(buffer.end(), ((properties.value(i) >> 8) & 0xff));
//        buffer.insert(buffer.end(), ((properties.value(i) >> 16) & 0xff));
//        buffer.insert(buffer.end(), ((properties.value(i) >> 24) & 0xff));
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
