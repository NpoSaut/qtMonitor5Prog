#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include "QByteArray"
#include "QDataStream"

typedef unsigned char byte;

enum MessageId
{
    progInit = 0x01,
    progStatus = 0x02,
    progListRq = 0x03,
    progList = 0x04,
    progReadRq = 0x05,
    progRead = 0x06,
    progRm = 0x07,
    progRmAck = 0x08,
    progCreate = 0x09,
    progCreateAck = 0x0a,
    progWrite = 0x0b,
    progMrProper = 0x0d,
    progMrProperRm = 0x0e,
    paramSetRq = 0x0f,
    paramSetAck = 0x10,
    paramRmRq = 0x11,
    paramRmAck = 0x12
};

class Message
{
public:
    static QByteArray fromVectorToQByteArray(const std::vector<byte> &data);
    static std::vector<byte> fromQByteArrayToVector(const QByteArray &arr);

protected:
    virtual std::vector<byte> encode() =0;
    virtual void decode(const std::vector<byte> &data) =0;
};

#endif // MESSAGE_H
