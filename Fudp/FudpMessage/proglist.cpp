#include "proglist.h"

ProgList::ProgList()
{
}

ProgList::ProgList(const QList<DevFileInfo> &listDevFileInfo) :
    listDevFileInfo(listDevFileInfo)
{
}

std::vector<byte> ProgList::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    for(QList<DevFileInfo>::iterator i = listDevFileInfo.begin(); i != listDevFileInfo.end(); i++)
    {
        DevFileInfo dfi = *i;
        in << (byte)MessageId(progList) << (byte)dfi.getFileNameSize() << dfi.getFileName("Windows-1251") << dfi.getFileSize() << dfi.getControlSum();
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

        DevFileInfo dfi(fName, fSize, controlSum);
        listDevFileInfo.append(dfi);
    }

}
