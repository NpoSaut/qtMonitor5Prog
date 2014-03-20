#ifndef PROGBROADCASTANSWER_H
#define PROGBROADCASTANSWER_H

#include "proginit.h"

namespace FudpMessage
{

class ProgBroadcastAnswer
{
public:
    ProgBroadcastAnswer();
    ProgBroadcastAnswer(DeviceTicket ticket);

    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    DeviceTicket getTicket();
//    qint8 getIdSystem();
//    qint16 getIdBlock();
//    qint8 getModificationOfBlock();

private:
    DeviceTicket ticket;
//    qint8 idSystem;
//    qint16 idBlock;
//    qint8 modificationOfBlock;
};

}

#endif // PROGBROADCASTANSWER_H
