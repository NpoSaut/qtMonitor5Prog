#ifndef PROGFIRMCORRUPT_H
#define PROGFIRMCORRUPT_H

#include "message.h"

namespace FudpMessage
{

class ProgFirmCorrupt : public Message
{
public:
    ProgFirmCorrupt();

    virtual std::vector<byte> encode();
    virtual void decode(const std::vector<byte> &data);
};

}


#endif // PROGFIRMCORRUPT_H
