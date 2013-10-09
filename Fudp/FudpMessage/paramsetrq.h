#ifndef PARAMSETRQ_H
#define PARAMSETRQ_H

#include "message.h"

namespace FudpMessage
{
class ParamSetRq : public Message
{
public:
    ParamSetRq();
    ParamSetRq(qint8 key, qint32 paramValue);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    qint8 getKey();
    qint32 getParamValue();

private:
    qint8 key;
    qint32 paramValue;
};
}

#endif // PARAMSETRQ_H
