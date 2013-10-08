#ifndef PROGRM_H
#define PROGRM_H

#include "message.h"
#include "../devfileinfo.h"

class ProgRm : public Message
{
public:
    ProgRm();
    ProgRm(DevFileInfo &file);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);
    QString getFileName();

private:
    QString fileName;
};

#endif // PROGRM_H
