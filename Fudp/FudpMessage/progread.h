#ifndef PROGREAD_H
#define PROGREAD_H

#include "QHash"
#include "message.h"
#include "../devfileinfo.h"

namespace FudpMessage
{
class ProgRead  : public Message
{
public:
    ProgRead();
    ProgRead(const QByteArray &fData, qint8 eCode);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);
    QByteArray getData();
    qint8 getErrorCode();

    enum ProgReadErrors
    {
        fileNotFound = 1,
        invalidOffset = 2,
        readError = 3
    };

private:
    QByteArray data;
    qint8 errorCode;
};
}

#endif // PROGREAD_H
