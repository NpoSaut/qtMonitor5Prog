#ifndef DEVFILEINFO_H
#define DEVFILEINFO_H

#include "QString"
#include "QTextCodec"


class DevFileInfo
{
public:
    DevFileInfo(qint32 fSize = 0);
    DevFileInfo(qint32 fSize, qint32 cSum);
    DevFileInfo(QByteArray data);

    bool setData(const QByteArray &data, int offset);
    void setFileName (QString &fileName) { this->fileName = fileName; }

    QByteArray getData();
    QByteArray getData(qint32 offset, qint32 readSize);
    qint32 getFileSize() const { return fileSize; }
    quint16 getControlSum() const { return controlSum; }
    qint8 getFileNameSize() const { return fileName.size (); }
    QString getFileName () const { return fileName; }

private:
    unsigned short crc_ccitt_update(unsigned short crc, unsigned char data);

    quint16 calcControlSumm();
    QString fileName;
    QByteArray fileData;
    qint32 fileSize;
    quint16 controlSum;
};

#endif // DEVFILEINFO_H
