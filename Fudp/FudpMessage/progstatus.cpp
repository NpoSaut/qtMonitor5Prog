#include "progstatus.h"

namespace FudpMessage
{
ProgStatus::ProgStatus()
{
}

ProgStatus::ProgStatus(const QVector<QPair<quint8, qint32> > &dictionary)    :
    properties(dictionary)
{
}

std::vector<byte> ProgStatus::encode()
{
    QByteArray arr;
    QDataStream in(&arr, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in << (byte)MessageId(progStatus);

    foreach (auto entry, properties)
    {
        in << entry.first << entry.second;
    }
    return Message::fromQByteArrayToVector(arr);
}

void ProgStatus::decode(const std::vector<byte> &data)
{
    QByteArray buffer = Message::fromVectorToQByteArray(data);
    QDataStream out (&buffer, QIODevice::ReadOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out.skipRawData(1);

    properties.clear();
    while(!out.atEnd())
    {
        qint8 key;
        qint32 value;
        out >> key;
        out >> value;
        properties.append(QPair<quint8, qint32> (key, value));
    }
}

QVector<QPair<quint8, qint32> > ProgStatus::getProperties()
{
    return properties;
}

}
