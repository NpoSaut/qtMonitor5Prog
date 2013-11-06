#ifndef PROGRMACK_H
#define PROGRMACK_H

#include "QHash"
#include "message.h"

namespace FudpMessage
{
class ProgRmAck : public Message
{
public:
    ProgRmAck();
    ProgRmAck(qint8 errCode);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);


    enum ProgRmErrors
    {
        FileNotExists = 1,
        deleteError = 2
    };

private:
    qint8 errorCode;
    QHash<int, QString> rmError;

};
}

#endif // PROGRMACK_H
