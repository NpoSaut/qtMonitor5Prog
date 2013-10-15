#include "paramsetrq.h"

namespace FudpMessage
{
ParamSetRq::ParamSetRq()
{
}

ParamSetRq::ParamSetRq(qint8 key, qint32 paramValue) :
    key(key), paramValue(paramValue)
{
}

std::vector<byte> ParamSetRq::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);

    in << MessageId(paramSetRq) << key << paramValue;

    return Message::fromQByteArrayToVector(buffer);
}

void ParamSetRq::decode(const std::vector<byte> &data)
{
    QByteArray buffer = Message::fromVectorToQByteArray(data);
    QDataStream out(&buffer, QIODevice::ReadOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out.skipRawData(1);

    out >> key;
    out >> paramValue;
}

qint8 ParamSetRq::getKey()
{
    return key;
}

qint32 ParamSetRq::getParamValue()
{
    return paramValue;
}
}
