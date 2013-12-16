#include "progsubmitack.h"

using namespace FudpMessage;

ProgSubmitAck::ProgSubmitAck(qint8 fCode)   :
    finalCode(fCode)
{
}

std::vector<byte> ProgSubmitAck::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), MessageId(progSubmitAck));
    buffer.insert(buffer.end(), finalCode);
    return buffer;
}

void ProgSubmitAck::decode(const std::vector<byte> &data)
{

}
