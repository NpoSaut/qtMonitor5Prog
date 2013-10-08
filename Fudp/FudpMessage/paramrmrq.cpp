#include "paramrmrq.h"

ParamRmRq::ParamRmRq()
{
}

ParamRmRq::ParamRmRq(qint8 key) :
    key(key)
{
}

std::vector<byte> ParamRmRq::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), (byte)MessageId(paramRmRq));
    buffer.insert(buffer.end(),(byte)key);

    return buffer;
}

void ParamRmRq::decode(const std::vector<byte> &data)
{
    key = data.at(1);
}

qint8 ParamRmRq::getKey()
{
    return key;
}
