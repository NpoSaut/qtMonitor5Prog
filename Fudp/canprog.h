#ifndef CANPROG_H
#define CANPROG_H

#include <QObject>
#include <QFile>
#include <QDir>
#include "FudpMessage/workingwithfudpmessage.h"

class CanProg : public QObject
{
    Q_OBJECT
public:
    explicit CanProg(QObject *parent = 0);
    
private:
    WorkingWithFudpMessage worker;

    qint8 systemId;
    qint16 blockId;
    qint8 blockModification;

    QHash<qint8, qint32> dictionary;
    QList<DevFileInfo> fileList;

signals:
    void sendProgStatus(const QHash<qint8, qint32> &dictionary);
    void sendFileList(const QList<DevFileInfo> &list);
    void sendFile(qint8 errorCode, const QByteArray &data);
    void sendDeleteFileAck(qint8 errorCode);
    void sendDeleteAllFilesAck();
    void sendCreateFileAck(qint8 errorCode);
    void sendSetParamAck(qint8 errorCode);
    void sendDeleteParamAck(qint8 errorCode);

public slots:
    void connect(qint8 systemId, qint16 blockId, qint8 modificationBLock);
    void getFileList();
    void readFile(const QString &fileName, qint32 offset, qint32 readSize);
    void deleteFile(const QString &fileName);
    void deleteAllFiles(qint32 securityKey);
    void createFile(const QString &fileName, qint32 fileSize);
    void writeFile(const QString &fileName, qint32 offset, const QByteArray &data);
    void setParam(qint8 key, qint32 value);
    void deleteParam(qint8 key);
};

#endif // CANPROG_H
