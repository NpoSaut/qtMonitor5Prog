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
    ProgList(QList<DevFileInfo> &listDevFileInfo);

    // Если весь список не помещается в максимальную длину сообщения,
    // то высылает максимально возможное кол-во записей и флаг наличия оставшихся файлов
    std::vector<byte> encode();
    void decode(const std::vector<byte> &data);

    bool isRemainder () const {return remainder;}

private:
    QList<DevFileInfo> listDevFileInfo;
    bool remainder;
};
}

#endif // PROGLIST_H
