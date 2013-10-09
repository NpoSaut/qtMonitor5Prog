#ifndef PROGSTATUS_H
#define PROGSTATUS_H

#include "QHash"
#include "message.h"

namespace FudpMessage
{
class ProgStatus : Message
{
public:
    ProgStatus();
    ProgStatus(const QHash<qint8, qint32> &properties);

    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    QHash<qint8,qint32> getProperties();

private:
    QHash<qint8,qint32> properties;
};
}

#endif // PROGSTATUS_H
