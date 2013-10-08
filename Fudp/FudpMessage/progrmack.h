#ifndef PROGRMACK_H
#define PROGRMACK_H

#include "QHash"
#include "message.h"

class ProgRmAck : public Message
{
public:
    ProgRmAck();
    ProgRmAck(qint8 errCode);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    qint8 errorCode;

    enum ProgRmErrors
    {
        FileNotExists = 1,
        deleteError = 2
    };

private:
    QHash<int, QString> rmError;

};

#endif // PROGRMACK_H
