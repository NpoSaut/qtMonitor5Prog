#ifndef PROGSUBMIT_H
#define PROGSUBMIT_H

#include "message.h"

namespace FudpMessage
{


class ProgSubmit : public Message
{
public:
    ProgSubmit();

    virtual std::vector<byte> encode();
    virtual void decode(const std::vector<byte> &data);
    qint8 applyCanges();

private:
    qint8 applyChanges;
};

}

#endif // PROGSUBMIT_H
