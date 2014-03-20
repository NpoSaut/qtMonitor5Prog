#include "proglist.h"

namespace FudpMessage
{
ProgList::ProgList()
{
}

ProgList::ProgList(QMap<QString, DevFileInfo> &listDevFileInfo) :
    listDevFileInfo(listDevFileInfo)
{
}

std::vector<byte> ProgList::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);

    auto keys = listDevFileInfo.keys();

    in << (byte)MessageId(progList);
    foreach(QString key, keys)
    {
        in << (byte)key.length();

        QByteArray t = Message::changeCodec(key, "Windows-1251");
        for (int i =0; i < t.size(); i ++)
            in << (quint8)t.at(i);

        in << listDevFileInfo[key].getFileSize() << (quint32) listDevFileInfo[key].getControlSum();
    }
    return Message::fromQByteArrayToVector(buffer);
}

void ProgList::decode(const std::vector<byte> &data)
{
    QByteArray buffer = Message::fromVectorToQByteArray(data);
    QDataStream out(&buffer, QIODevice::ReadOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    qint8 fNameSize = 0;
    for(int i = 1; i < buffer.size(); i + 10 + fNameSize)
    {

        out >> fNameSize;
        char *temp = new char[fNameSize];
        out.readRawData(temp, fNameSize);
        QString fName(temp);
        delete [] temp;

        qint32 fSize;
        out >> fSize;

        qint32 controlSum;
        out >> controlSum;

        DevFileInfo dfi(fSize, controlSum);
        listDevFileInfo.insert(fName, dfi);
    }

}
}
