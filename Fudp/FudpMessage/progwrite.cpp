#include "progwrite.h"

namespace FudpMessage
{
ProgWrite::ProgWrite()
    : fileName(), fileData(), offset(0)
{
}

ProgWrite::ProgWrite(QString fileName, DevFileInfo &file, qint32 offset) :
    fileName(Message::changeCodec(fileName, "Windows-1251")), fileData(file.getData()), offset(offset)
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

    QByteArray fNameArr(fileNameSize, Qt::Uninitialized);
    out.readRawData(fNameArr.data(), fileNameSize);
    fileName = QString(fNameArr);

    out >> offset;

    QByteArray fDataArr(buffer.size() - getValueOfOverhadsBytes(), Qt::Uninitialized);
    out.readRawData(fDataArr.data(), buffer.size() - getValueOfOverhadsBytes());
    fileData.append(fDataArr);

//    qDebug()<< "Write file" << fileName <<". offset: " << offset << "; len: " << fileData.length() << ";";
//    qDebug("data: %02x %02x %02x %02x ... ", (quint8)fileData.at(0), (quint8)fileData.at(1), (quint8)fileData.at(2), (quint8)fileData.at(3));
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
}
