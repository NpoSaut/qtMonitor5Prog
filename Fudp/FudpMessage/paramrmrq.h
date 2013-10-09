#ifndef PARAMRMRQ_H
#define PARAMRMRQ_H

#include "message.h"

namespace FudpMessage
{
class ParamRmRq : public Message
{
public:
    ParamRmRq();
    ParamRmRq(qint8 key);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    qint8 getKey();
private:
    qint8 key;
};
}

#endif // PARAMRMRQ_H
