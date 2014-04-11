#ifndef PROGPONG_H
#define PROGPONG_H

#include "message.h"

namespace FudpMessage
{
class ProgPong : public Message
{
public:
    enum Status
    {
        ALIVE = 0,
        SINGLE_ERROR = 1,
        MULTIPLE_ERROR = 2
    };
    ProgPong(quint8 counter = 0, Status status = ALIVE);

    void setCounter (quint8 counter) { this->counter = counter; }
    void setStatus (Status status) { this->status = status; }
    quint8 getCounter () const { return counter; }
    Status getStatus () const { return status; }

    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

protected:
    quint8 counter;
    Status status;
};
}

#endif // PROGPONG_H
