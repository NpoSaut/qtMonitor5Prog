#ifndef PROGWRITE_H
#define PROGWRITE_H

#include "message.h"
#include "../devfileinfo.h"

namespace FudpMessage
{
class ProgWrite : public Message
{
public:
    ProgWrite();
    ProgWrite(QString fileName, DevFileInfo &file, qint32 offset);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    QString getFileName();
    qint32 getOffset();
    QByteArray getData();

private:
    qint32 getWriteBufferSize();
    qint32 getValueOfOverhadsBytes();

    QString fileName;
    qint32 offset;
    QByteArray fileData;
    static const qint32 dataSize = 4000;
};
}

#endif // PROGWRITE_H
