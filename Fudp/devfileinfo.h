#ifndef DEVFILEINFO_H
#define DEVFILEINFO_H

#include "QString"
#include "QTextCodec"


class DevFileInfo
{
public:
    DevFileInfo();
    DevFileInfo(qint32 fSize);
    DevFileInfo(qint32 fSize, qint32 cSum);
    DevFileInfo(QByteArray data);

    bool setData(const QByteArray &data, int offset);
    QByteArray getData();

    QByteArray getData(qint32 offset, qint32 readSize);
    qint32 getFileSize();
    quint16 getControlSum();
    qint8 getFileNameSize();

private:
    unsigned short crc_ccitt_update(unsigned short crc, unsigned char data);

    quint16 calcControlSumm();
    QString fileName;
    QByteArray fileData;
    qint32 fileSize;
    quint16 controlSum;
};

#endif // DEVFILEINFO_H
