#ifndef PROGREADRQ_H
#define PROGREADRQ_H

#include "message.h"
#include "../devfileinfo.h"

class ProgReadRq : public Message
{
public:
    ProgReadRq();
    ProgReadRq(const QString &fName, qint32 offset, qint32 rSize);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    QString getFileName();
    qint32 getOffset();
    qint32 getReadSize();


private:
    qint32 setReadSize();

    QString fileName;
    qint32 offset;
    qint32 readSize;
};

#endif // PROGREADRQ_H
