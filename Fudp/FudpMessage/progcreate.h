#ifndef PROGCREATE_H
#define PROGCREATE_H

#include "message.h"

namespace FudpMessage
{
class ProgCreate : public Message
{
public:
    ProgCreate();
    ProgCreate(const QString &fName, qint32 fSize);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    QString getFileName();
    qint32 getFileSize();

private:
    QString fileName;
    qint32 fileSize;
};
}

#endif // PROGCREATE_H
