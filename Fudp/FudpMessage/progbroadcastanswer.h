#ifndef PROGBROADCASTANSWER_H
#define PROGBROADCASTANSWER_H

#include "proginit.h"

namespace FudpMessage
{

class ProgBroadcastAnswer
{
public:
    ProgBroadcastAnswer();
    ProgBroadcastAnswer(DeviceTickets ticket);

    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    DeviceTickets getTicket();
//    qint8 getIdSystem();
//    qint16 getIdBlock();
//    qint8 getModificationOfBlock();

private:
    DeviceTickets ticket;
//    qint8 idSystem;
//    qint16 idBlock;
//    qint8 modificationOfBlock;
};

}

#endif // PROGBROADCASTANSWER_H
