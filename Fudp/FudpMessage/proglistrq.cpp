#include "proglistrq.h"

namespace FudpMessage
{
ProgListRq::ProgListRq(quint16 offset, quint16 count)
    : offset (offset), count (count)
{ }

std::vector<byte> ProgListRq::encode()
{
    std::vector<byte> buffer (0, 5);
    buffer[0] = MessageId(progListRq);
    buffer[1] = offset;
    buffer[2] = offset/256;
    buffer[3] = count;
    buffer[4] = count/256;
    return buffer;
}

void ProgListRq::decode(const std::vector<byte> &data)
{
    setOffset (data.size () >= 3 ? data[1] + data[2]*256 : 0);
    setCount  (data.size () >= 5 ? data[3] + data[4]*256 : 0);
}
}
