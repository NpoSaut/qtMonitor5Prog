#ifndef PROGINIT_H
#define PROGINIT_H

#include "message.h"

namespace FudpMessage
{
class ProgInit : public Message
{
public:
    ProgInit();
    ProgInit(qint8 idSystem, qint16 idBlock, qint8 modificationOfBlock);

    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    qint8 getIdSystem();
    qint16 getIdBlock();
    qint8 getModificationOfBlock();

private:
    qint8 idSystem;
    qint16 idBlock;
    qint8 modificationOfBlock;

};
}

#endif // PROGINIT_H
