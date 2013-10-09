#ifndef PROGCREATEACK_H
#define PROGCREATEACK_H

#include "message.h"
#include "QHash"

namespace FudpMessage
{
class ProgCreateAck : public Message
{
public:
    ProgCreateAck();
    ProgCreateAck(qint8 errCode);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    qint8 errorCode;

    enum ProgCreateAckErrors
    {
        FileAlreadyExists = 1,
        FileMaximumNumber = 2,
        OutOfMemry = 3,
        ErrorCreate = 4
    };

private:
    QHash<qint8, QString> createErrors;

};
}

#endif // PROGCREATEACK_H
