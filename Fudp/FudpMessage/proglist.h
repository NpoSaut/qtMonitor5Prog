#ifndef PROGLIST_H
#define PROGLIST_H

#include "QTextCodec"
#include "message.h"
#include "QList"
#include "../devfileinfo.h"

namespace FudpMessage
{
class ProgList : public Message
{
public:
    ProgList();
    ProgList(QMap<QString, DevFileInfo> &listDevFileInfo);
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

private:
    QMap<QString, DevFileInfo> listDevFileInfo;


};
}

#endif // PROGLIST_H
