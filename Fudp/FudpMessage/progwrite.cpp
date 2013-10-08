#include "progwrite.h"

ProgWrite::ProgWrite()
{
}

ProgWrite::ProgWrite(DevFileInfo &file, qint32 offset) :
    fileName(file.getFileName("Windows-1251")), fileData(file.getData("Windows-1251")), offset(offset)
{
}

std::vector<byte> ProgWrite::encode()
{
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    in << MessageId(progWrite) << (byte)fileName.size() << fileName << offset << fileData.mid(offset, getWriteBufferSize());

    return Message::fromQByteArrayToVector(buffer);
}

void ProgWrite::decode(const std::vector<byte> &data)
{
    QByteArray buffer = Message::fromVectorToQByteArray(data);
    QDataStream out(&buffer, QIODevice::ReadOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out.skipRawData(1);

    qint8 fileNameSize;
    out >> fileNameSize;

    char *temp = new char[fileNameSize];
    out.readRawData(temp, fileNameSize);
    fileName.append(temp);
    delete temp;

    out >> offset;

    char *temp2 = new char[buffer.size() - getValueOfOverhadsBytes()];
    out.readRawData(temp2, buffer.size() - getValueOfOverhadsBytes());
    fileData.append(temp2);
    delete temp;
}

qint32 ProgWrite::getWriteBufferSize()
{
    return fileData.size() - offset > dataSize + getValueOfOverhadsBytes() ?
                dataSize + getValueOfOverhadsBytes() : fileData.size() - offset;
}

qint32 ProgWrite::getValueOfOverhadsBytes()
{
    return 6 + fileName.size();
}

QString ProgWrite::getFileName()
{
    return fileName;
}

qint32 ProgWrite::getOffset()
{
    return offset;
}

QByteArray ProgWrite::getData()
{
    return fileData;
}
