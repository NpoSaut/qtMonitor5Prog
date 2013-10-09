#ifndef PROGLISTRQ_H
#define PROGLISTRQ_H

#include "message.h"

namespace FudpMessage
{
class ProgListRq : public Message
{
public:
    ProgListRq();

    std::vector<byte> encode();
    void decode(std::vector<byte> &data);
};
}

#endif // PROGLISTRQ_H
