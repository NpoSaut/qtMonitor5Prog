#include "message.h"
#include <QTextCodec>

namespace FudpMessage
{
std::vector<byte> Message::fromQByteArrayToVector(const QByteArray &arr)
{
    std::vector<byte> data;
    for(int i = 0; i<arr.size(); i++)
    {
        data.insert(data.end(), (byte)arr.at(i));
    }

    return data;
}

QByteArray Message::fromVectorToQByteArray(const std::vector<byte> &data)
{
    QByteArray arr;
    for(int i = 0; i<data.size(); i++)
        arr.append((byte)data.at(i));
    return arr;
}

QByteArray Message::changeCodec(QString str, QString newCodec)
{
    QTextCodec *codec = QTextCodec::codecForName(newCodec.toLatin1());
    QByteArray encodingStr = codec->fromUnicode(str);
    return encodingStr;
}
}
