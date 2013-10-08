#include "message.h"

std::vector<byte> Message::fromQByteArrayToVector(const QByteArray &arr)
{
    std::vector<byte> data;
    for(int i = 0; i<arr.size(); i++)
        data.insert(data.end(), (byte)arr.at(i));

    return data;
}

QByteArray Message::fromVectorToQByteArray(const std::vector<byte> &data)
{
    QByteArray arr;
    qDebug("%x", data.at(0));
    for(int i = 0; i<data.size(); i++)
        arr.append((byte)data.at(i));
    return arr;
}
