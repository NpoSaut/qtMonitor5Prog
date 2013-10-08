#ifndef PARAMRMACK_H
#define PARAMRMACK_H

#include "message.h"

class ParamRmAck : public Message
{
public:
    ParamRmAck();
    ParamRmAck(qint8 errCode);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    qint8 getErrorCode();

    enum ParamRmAckError
    {
        ReadOnlyParam = 1,
        ParamNotFound = 2
    };
private:
    qint8 errorCode;
};

#endif // PARAMRMACK_H
