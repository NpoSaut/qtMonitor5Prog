#include "devfileinfo.h"
#include "Log/logwriter.h"

DevFileInfo::DevFileInfo(QString name, qint32 fSize, qint32 cSum) :
    fileName (name), fileSize(fSize), controlSum(cSum), fileData (fSize, 0)
{
}

DevFileInfo::DevFileInfo(QString name, QByteArray data) :
    fileName (name), fileData(data), fileSize(data.size()), controlSum(calcControlSumm())
{

}

bool DevFileInfo::setData(const QByteArray &data, int offset)
{
    if ( data.length () + offset <= fileSize )
    {
        fileData.replace (offset, data.length (), data);
        controlSum = calcControlSumm();
        return true;
    }
    else
        return false;
}

QByteArray DevFileInfo::getData()
{
    return fileData;
}

QByteArray DevFileInfo::getData(qint32 offset, qint32 readSize)
{
    return fileData.mid(offset, readSize);
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

