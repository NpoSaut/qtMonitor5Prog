#include "progwriteack.h"

using namespace FudpMessage;

ProgWriteAck::ProgWriteAck()
{
}

ProgWriteAck::ProgWriteAck(qint8 errCode) :
    errorCode(errCode)
{

}

std::vector<byte> ProgWriteAck::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), MessageId(progWriteAck));
    buffer.insert(buffer.end(), errorCode);
    return buffer;
}

void ProgWriteAck::decode(const std::vector<byte> &data)
{
    errorCode = data.at(1);
}
