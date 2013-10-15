#include "devfileinfo.h"


DevFileInfo::DevFileInfo()
{
}

DevFileInfo::DevFileInfo(QString name, qint32 fSize, qint32 cSum) :
    fileName(name), fileSize(fSize), controlSum(cSum)
{
}

DevFileInfo::DevFileInfo(QString name, QByteArray data) :
    fileName(name), fileData(data), fileSize(data.size()), controlSum(calcControlSumm())
{

}

QByteArray DevFileInfo::getFileName(QString newCodec)
{
    QTextCodec *codec = QTextCodec::codecForName(newCodec.toLatin1());
    QByteArray encodingFileName = codec->fromUnicode(fileName);
    return encodingFileName;
}

QByteArray &DevFileInfo::getData(QString newCodec)
{
    return fileData;
}

qint32 DevFileInfo::getFileSize()
{
    return fileSize;
}

quint16 DevFileInfo::getControlSum()
{
    return controlSum;
}

qint8 DevFileInfo::getFileNameSize()
{
    return fileName.length();
}

quint16 DevFileInfo::calcControlSumm()
{
    unsigned short crc = 0xffff;
    for(int i = 0; i < fileData.size(); i++)
    {
        crc = this->crc_ccitt_update((unsigned short)crc, (unsigned char)fileData.at(i));
    }
    return crc;
}

unsigned short DevFileInfo::crc_ccitt_update(unsigned short crc, unsigned char data)
{
    unsigned short t;
    data ^= crc&255;
    data ^= data << 4;
    t = (((unsigned short)data << 8) | ((crc >> 8) & 255));
    t ^= (unsigned char)(data >> 4);
    t ^= ((unsigned short)data << 3);
    return t;
}

