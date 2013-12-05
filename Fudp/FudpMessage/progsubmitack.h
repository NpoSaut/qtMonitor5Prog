#ifndef PROGSUBMITACK_H
#define PROGSUBMITACK_H

#include "message.h"

namespace FudpMessage
{
class ProgSubmitAck : public Message
{
public:
    ProgSubmitAck(qint8 fCode);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

private:
    qint8 finalCode;
};
}
#endif // PROGSUBMITACK_H
