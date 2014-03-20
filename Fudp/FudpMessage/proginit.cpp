#include "proginit.h"

namespace FudpMessage
{
ProgInit::ProgInit()
{
}

ProgInit::ProgInit(DeviceTicket ticket)  :
    ticket(ticket)
{
}

std::vector<byte> ProgInit::encode()
{
    QByteArray arr;
    QDataStream in(&arr, QIODevice::ReadWrite);
    in.setByteOrder(QDataStream::LittleEndian);
    in << (byte)MessageId(progInit)
       << byte((ticket.blockId & 0xff0) >> 4)
       << byte((ticket.blockId & 0x00f) << 4 | ticket.modification & 0x0f)
       << byte(ticket.module)
       << byte((ticket.channel & 0x0f) << 4 | (ticket.blockSerialNumber & 0xf0000) >> 16)
       << byte((ticket.blockSerialNumber & 0x0ff00) >> 8)
       << byte(ticket.blockSerialNumber & 0x000ff);

    return Message::fromQByteArrayToVector(arr);
}

void ProgInit::decode(const std::vector<byte> &data)
{
    this->ticket.blockId = (data.at(1) << 4) | ((data.at(2) & 0xf0) >> 4);
    this->ticket.modification = data.at(2) & 0x0f;
    this->ticket.module = data.at(3);
    this->ticket.channel = (data.at(4) & 0xf0) >> 4;
    this->ticket.blockSerialNumber = (((data.at(4) & 0x0f) << 16) | (data.at(5) << 8) | data.at(6));
}

DeviceTicket ProgInit::getTicket()
{
    return ticket;
}

bool DeviceTicket::operator ==(const DeviceTicket &ticket)
{
    return this->modification == ticket.modification && this->blockId == ticket.blockId &&
            this->blockSerialNumber == ticket.blockSerialNumber && this->channel == ticket.channel &&
            this->module == ticket.module;
}

bool DeviceTicket::operator <=(const DeviceTicket &ticket)
{
    return (this->modification == ticket.modification           || ticket.modification == 0)
        && (this->blockId == ticket.blockId                     || ticket.blockId == 0)
        && (this->blockSerialNumber == ticket.blockSerialNumber || ticket.blockSerialNumber == 0)
        && (this->channel == ticket.channel                     || ticket.channel == 0)
        && (this->module == ticket.module                       || ticket.module == 0);
}

}
