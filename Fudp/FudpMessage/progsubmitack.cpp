#include "progsubmitack.h"

using namespace FudpMessage;

ProgSubmitAck::ProgSubmitAck()
{
}

std::vector<byte> ProgSubmitAck::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), MessageId(progSubmitAck));
    return buffer;
}

void ProgSubmitAck::decode(const std::vector<byte> &data)
{

}
