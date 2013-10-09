#include "progmrproperack.h"

namespace FudpMessage
{
ProgMrProperAck::ProgMrProperAck()
{
}

std::vector<byte> ProgMrProperAck::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), MessageId(progMrProperRm));
    return buffer;
}

void ProgMrProperAck::decode(const std::vector<byte> &data)
{

}
}
