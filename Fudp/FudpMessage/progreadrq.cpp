#include "progreadrq.h"

namespace FudpMessage
{
ProgReadRq::ProgReadRq()
{
}

ProgReadRq::ProgReadRq(const QString &fName, qint32 offset, qint32 rSize)  :
    fileName(fName), offset(offset), readSize(rSize)
{

}

std::vector<byte> ProgReadRq::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in << (byte)MessageId(progReadRq) << fileName.size() << fileName << offset << setReadSize();
    return Message::fromQByteArrayToVector(buffer);
}

void ProgReadRq::decode(const std::vector<byte> &data)
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

    out >> offset;

    out >> readSize;
}

qint32 ProgReadRq::setReadSize()
{
    return readSize > 4000 ?
                4000 : readSize;
}

QString ProgReadRq::getFileName()
{
    return fileName;
}

qint32 ProgReadRq::getOffset()
{
    return offset;
}

qint32 ProgReadRq::getReadSize()
{
    return readSize;
}
}
