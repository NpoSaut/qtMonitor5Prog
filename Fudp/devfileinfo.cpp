#include "devfileinfo.h"
#include "Log/logwriter.h"

DevFileInfo::DevFileInfo()
{
}

DevFileInfo::DevFileInfo(qint32 fSize, qint32 cSum) :
    fileSize(fSize), controlSum(cSum)
{
}

DevFileInfo::DevFileInfo(QByteArray data) :
    fileData(data), fileSize(data.size()), controlSum(calcControlSumm())
{

}

DevFileInfo::DevFileInfo(qint32 fSize) :
    fileSize(fSize)
{

}

bool DevFileInfo::setData(const QByteArray &data, int offset)
{
    if(fileData.length() + data.length() > fileSize)
        return false;
    if (offset ==0 && fileData.size()>0)
        fileData.clear();
    fileData.insert(offset, data);
    controlSum = calcControlSumm();
    return true;
}

QByteArray DevFileInfo::getData()
{
    return fileData;
}

QByteArray DevFileInfo::getData(qint32 offset, qint32 readSize)
{
    return fileData.mid(offset, offset+readSize);
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

