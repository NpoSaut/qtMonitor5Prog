#include "progcreate.h"

namespace FudpMessage
{
ProgCreate::ProgCreate()
{
}

ProgCreate::ProgCreate(const QString &fName, qint32 fSize) :
    fileName(fName), fileSize(fSize)
{
}

std::vector<byte> ProgCreate::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in << (byte)MessageId(progCreate) << (byte)fileName.size() << fileName << fileSize;

    return Message::fromQByteArrayToVector(buffer);
}

void ProgCreate::decode(const std::vector<byte> &data)
{
    QByteArray buffer = Message::fromVectorToQByteArray(data);
    QDataStream out(&buffer, QIODevice::ReadOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out.skipRawData(1);

    qint8 fileNameSize;
    out >> fileNameSize;

    QByteArray arr(fileNameSize, Qt::Uninitialized);
    out.readRawData(arr.data(), fileNameSize);
    fileName.append(arr);

    out >> fileSize;
}

QString ProgCreate::getFileName()
{
    return fileName;
}

qint32 ProgCreate::getFileSize()
{
    return fileSize;
}
}
