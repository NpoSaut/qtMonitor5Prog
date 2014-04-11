#include "progping.h"

using namespace FudpMessage;

FudpMessage::ProgPing::ProgPing(quint8 counter)
    : counter (counter)
{ }

std::vector<byte> ProgPing::encode()
{
    std::vector<byte> out (2, 0);
    out[0] = progPing;
    out[1] = counter;
    return out;
}

void ProgPing::decode(const std::vector<byte> &data)
{
    counter = data[1];
}
