#include "progrm.h"

namespace FudpMessage
{
ProgRm::ProgRm()
{
}

ProgRm::ProgRm(DevFileInfo &file) :
    fileName(file.getFileName("Windows-1251"))
{
}

std::vector<byte> ProgRm::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::ReadWrite);
    in.setByteOrder(QDataStream::LittleEndian);
    in << (byte)MessageId(progRm) << fileName.size() << fileName;

    return Message::fromQByteArrayToVector(buffer);
}

void ProgRm::decode(const std::vector<byte> &data)
{
    QByteArray buffer = Message::fromVectorToQByteArray(data);
    QDataStream out(&buffer, QIODevice::ReadWrite);
    out.setByteOrder(QDataStream::LittleEndian);
    out.skipRawData(1);

    qint8 fileNameSize;
    out >> fileNameSize;

    QByteArray arr(fileNameSize, Qt::Uninitialized);
    out.readRawData(arr.data(), fileNameSize);
    fileName.append(arr);
}

QString ProgRm::getFileName()
{
    return fileName;
}
}
