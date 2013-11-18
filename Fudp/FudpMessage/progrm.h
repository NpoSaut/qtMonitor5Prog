#ifndef PROGRM_H
#define PROGRM_H

#include "message.h"
#include "../devfileinfo.h"

namespace FudpMessage
{
class ProgRm : public Message
{
public:
    ProgRm();
    ProgRm(const QString fileName);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);
    QString getFileName();

private:
    QString fileName;
};
}

#endif // PROGRM_H
