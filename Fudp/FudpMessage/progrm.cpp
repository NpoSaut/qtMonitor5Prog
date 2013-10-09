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
    out.skipRawData(2);

    char *temp = new char[buffer.at(1)];
    out.readRawData(temp, buffer.at(1));
    fileName.append(temp);
}

QString ProgRm::getFileName()
{
    return fileName;
}
}
