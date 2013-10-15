#ifndef PROGINIT_H
#define PROGINIT_H

#include "message.h"

namespace FudpMessage
{

struct DeviceTickets
{
    qint32 blockId;
    qint32 blockSerialNumber;
    qint32 module;
    qint32 channel;
    qint32 modification;

    bool operator == (const DeviceTickets &ticket);
    bool operator <= (const DeviceTickets &ticket); // Если подходим по маске
};

class ProgInit : public Message
{
public:
    ProgInit();
    ProgInit(DeviceTickets ticket);

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

#endif // PROGINIT_H
