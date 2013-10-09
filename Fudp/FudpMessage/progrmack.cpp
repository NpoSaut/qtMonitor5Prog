#include "progrmack.h"

namespace FudpMessage
{
ProgRmAck::ProgRmAck()
{
    rmError[1] = "File not found";
    rmError[2] = "Error";
}

ProgRmAck::ProgRmAck(qint8 errCode) :
    errorCode(errCode)
{

}

std::vector<byte> ProgRmAck::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), MessageId(progRmAck));
    buffer.insert(buffer.end(), errorCode);
    return buffer;
}

void ProgRmAck::decode(const std::vector<byte> &data)
{
    errorCode = data.at(1);
}
}
