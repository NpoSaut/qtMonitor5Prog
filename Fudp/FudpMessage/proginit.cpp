#include "proginit.h"

namespace FudpMessage
{
ProgInit::ProgInit()
{
}

ProgInit::ProgInit(qint8 idSystem, qint16 idBlock, qint8 modificationOfBlock)  :
    idSystem(idSystem), idBlock(idBlock), modificationOfBlock(modificationOfBlock)
{
}

std::vector<byte> ProgInit::encode()
{
    QByteArray arr;
    QDataStream in(&arr, QIODevice::ReadWrite);
    in.setByteOrder(QDataStream::LittleEndian);
    in << (byte)MessageId(progInit) << idSystem << idBlock << modificationOfBlock << (byte)0xf2 << (byte)0x5b;

    return Message::fromQByteArrayToVector(arr);
}

void ProgInit::decode(const std::vector<byte> &data)
{
    this->idSystem = data[1];
    this->idBlock = data[2] << 8 | data[3];
    this->modificationOfBlock = data[4];
}

qint8 ProgInit::getIdSystem()
{
    return idSystem;
}

qint16 ProgInit::getIdBlock()
{
    return idBlock;
}

qint8 ProgInit::getModificationOfBlock()
{
    return modificationOfBlock;
}
}
