#ifndef PROGWRITEACK_H
#define PROGWRITEACK_H

#include "message.h"

namespace FudpMessage
{
class ProgWriteAck : public Message
{
public:
    ProgWriteAck();
    ProgWriteAck(qint8 errorCode);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);


    enum ProgWriteErrors
    {
        outOfRange = 1,
        unknowErroe = 255
    };

private:
    qint8 errorCode;
};
}
#endif // PROGWRITEACK_H
