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
    std::vector<byte> out (2, 0);
    out[0] = progRead;
    out[1] = errorCode;
    if (errorCode == 0)
    {
        std::vector<byte> dat = Message::fromQByteArrayToVector (data);
        out.insert (out.end(), dat.begin(), dat.end());
    }

    return out;
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
