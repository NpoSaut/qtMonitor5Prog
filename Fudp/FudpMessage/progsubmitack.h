#ifndef PROGSUBMITACK_H
#define PROGSUBMITACK_H

#include "message.h"

namespace FudpMessage
{
class ProgSubmitAck : public Message
{
public:
    ProgSubmitAck();
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);
};
}
#endif // PROGSUBMITACK_H
