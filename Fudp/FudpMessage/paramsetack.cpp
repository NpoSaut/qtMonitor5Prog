#include "paramsetack.h"

ParamSetAck::ParamSetAck()
{
}

ParamSetAck::ParamSetAck(qint8 errCode) :
    errorCode(errCode)
{
}

std::vector<byte> ParamSetAck::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), (byte)MessageId(paramSetAck));
    buffer.insert(buffer.end(), (byte)errorCode);

    return buffer;
}

void ParamSetAck::decode(const std::vector<byte> &data)
{
    errorCode = data.at(1);
}

qint8 ParamSetAck::getErrorCode()
{
    return errorCode;
}
