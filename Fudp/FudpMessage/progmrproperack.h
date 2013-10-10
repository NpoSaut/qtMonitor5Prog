#ifndef PROGMRPROPERACK_H
#define PROGMRPROPERACK_H

#include "message.h"

namespace FudpMessage
{
class ProgMrProperAck : public Message
{
public:
    ProgMrProperAck();
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);
};
}

#endif // PROGMRPROPERACK_H
