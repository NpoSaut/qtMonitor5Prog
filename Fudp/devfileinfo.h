#ifndef DEVFILEINFO_H
#define DEVFILEINFO_H

#include <QString>
#include "QTextCodec"

class DevFileInfo
{
public:
    DevFileInfo();
    DevFileInfo(QString name, qint32 fSize, qint32 cSum);
    DevFileInfo(QString name, QByteArray data);
    QByteArray getFileName(QString newCodec = "UTF-8");
    QByteArray &getData(QString newCodec = "UTF-8");
    qint32 getFileSize();
    qint32 getControlSum();
    qint8 getFileNameSize();

private:
    ushort calcControlSumm();
    QString fileName;
    QByteArray fileData;
    qint32 fileSize;
    qint8 controlSum;
};

#endif // DEVFILEINFO_H
