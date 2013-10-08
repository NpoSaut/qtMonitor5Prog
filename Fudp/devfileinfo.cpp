#include "devfileinfo.h"

DevFileInfo::DevFileInfo()
{
}

DevFileInfo::DevFileInfo(QString name, qint32 fSize, qint32 cSum) :
    fileName(name), fileSize(fSize), controlSum(cSum)
{
}

DevFileInfo::DevFileInfo(QString name, QByteArray data) :
    fileName(name), fileData(data), fileSize(data.size())
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

qint32 DevFileInfo::getControlSum()
{
    return controlSum;
}

qint8 DevFileInfo::getFileNameSize()
{
    return fileName.size();
}
