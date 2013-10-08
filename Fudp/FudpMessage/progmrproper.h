#ifndef PROGMRPROPER_H
#define PROGMRPROPER_H

#include "message.h"

class ProgMrProper : public Message
{
public:
    ProgMrProper();
    ProgMrProper(qint32 securityKey);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

private:
    qint32 securityKey;
};

#endif // PROGMRPROPER_H
