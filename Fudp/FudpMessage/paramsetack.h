#ifndef PARAMSETACK_H
#define PARAMSETACK_H

#include "message.h"

class ParamSetAck : public Message
{
public:
    ParamSetAck();
    ParamSetAck(qint8 errCode);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);
    qint8 getErrorCode();

    enum ParamSetAckError
    {
        ReadOnlyParam = 1,
        ParamMaxNumber = 2
    };

private:
    qint8 errorCode;
};

#endif // PARAMSETACK_H
