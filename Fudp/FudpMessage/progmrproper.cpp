#include "progmrproper.h"

ProgMrProper::ProgMrProper(qint32 securityKey) :
    securityKey(securityKey)
{
}

std::vector<byte> ProgMrProper::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in << MessageId(progMrProper) << securityKey;
}

void ProgMrProper::decode(const std::vector<byte> &data)
{
    QByteArray buffer;
    QDataStream out (&buffer, QIODevice::ReadOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out.skipRawData(1);
    out >> securityKey;
}
