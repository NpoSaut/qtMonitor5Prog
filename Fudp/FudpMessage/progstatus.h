#ifndef PROGSTATUS_H
#define PROGSTATUS_H

#include <QPair>
#include <QVector>
#include "message.h"

namespace FudpMessage
{
class ProgStatus : Message
{
public:
    ProgStatus();
    ProgStatus(const QVector< QPair<quint8, qint32> > &dictionary);

    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    QVector< QPair<quint8, qint32> > getProperties();

private:
    QVector< QPair<quint8, qint32> > properties;
};
}

#endif // PROGSTATUS_H
