#ifndef PROGLISTRQ_H
#define PROGLISTRQ_H

#include "message.h"

namespace FudpMessage
{
class ProgListRq : public Message
{
public:
    ProgListRq(quint16 offset = 0, quint16 count = 0);

    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    quint16 getOffset () const { return offset; }
    quint16 getCount () const { return count; }

    void setOffset (quint16 offset) { this->offset = offset; }
    void setCount (quint16 count) { this->count = count; }

private:
    quint16 offset;
    quint16 count;
};
}

#endif // PROGLISTRQ_H
