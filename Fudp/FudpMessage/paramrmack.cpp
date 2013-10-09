#include "paramrmack.h"
namespace FudpMessage
{
ParamRmAck::ParamRmAck()
{
}

ParamRmAck::ParamRmAck(qint8 errCode) :
    errorCode(errCode)
{
}

std::vector<byte> ParamRmAck::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), (byte)MessageId(paramRmAck));
    buffer.insert(buffer.end(), (byte)errorCode);

    return buffer;
}

void ParamRmAck::decode(const std::vector<byte> &data)
{
    errorCode = data.at(1);
}
}
