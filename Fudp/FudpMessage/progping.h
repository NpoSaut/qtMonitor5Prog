#ifndef PROGPING_H
#define PROGPING_H

#include "message.h"

namespace FudpMessage
{
class ProgPing : public Message
{
public:
    ProgPing(quint8 counter = 0);

    void setCounter (quint8 counter) { this->counter = counter; }
    void counterPlusPlus () { counter ++; }
    quint8 getCounter () const { return counter; }

    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

protected:
    quint8 counter;
};
}

#endif // PROGPING_H
