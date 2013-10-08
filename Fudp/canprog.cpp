#include "canprog.h"

CanProg::CanProg(QObject *parent) :
    QObject(parent), worker()
{
    QObject::connect(this, SIGNAL(sendProgStatus(QHash<qint8,qint32>)), &worker, SLOT(sendProgStatus(QHash<qint8,qint32>)));
    QObject::connect(this, SIGNAL(sendFileList(QList<DevFileInfo>)), &worker, SLOT(sendProgList(QList<DevFileInfo>)));
    QObject::connect(this, SIGNAL(sendFile(qint8,QByteArray)), &worker, SLOT(sendProgRead(qint8,QByteArray)));
    QObject::connect(this, SIGNAL(sendDeleteFileAck(qint8)), &worker, SLOT(sendProgRmAck(qint8)));
    QObject::connect(this, SIGNAL(sendDeleteAllFilesAck()), &worker, SLOT(sendProgMrPropperAck()));
    QObject::connect(this, SIGNAL(sendCreateFileAck(qint8)), &worker, SLOT(sendProgCreateAck(qint8)));
    QObject::connect(this, SIGNAL(sendSetParamAck(qint8)), &worker, SLOT(sendParamSetAck(qint8)));
    QObject::connect(this, SIGNAL(sendDeleteParamAck(qint8)), &worker, SLOT(sendParamRmAck(qint8)));

    QObject::connect(&worker, SIGNAL(getProgInit(qint8,qint16,qint8)), this, SLOT(connect(qint8,qint16,qint8)));
    QObject::connect(&worker, SIGNAL(getProgListRq()), this, SLOT(getFileList()));
    QObject::connect(&worker, SIGNAL(getProgReadRq(QString,qint32,qint32)), this, SLOT(readFile(QString,qint32,qint32)));
    QObject::connect(&worker, SIGNAL(getProgRm(QString)), this, SLOT(deleteFile(QString)));
    QObject::connect(&worker, SIGNAL(getProgMrPropper(qint32)), this, SLOT(deleteAllFiles(qint32)));
    QObject::connect(&worker, SIGNAL(getProgCreate(QString,qint32)), this, SLOT(createFile(QString,qint32)));
    QObject::connect(&worker, SIGNAL(getProgWrite(QString,qint32,QByteArray)), this, SLOT(writeFile(QString,qint32,QByteArray)));
    QObject::connect(&worker, SIGNAL(getParamSetRq(qint8,qint32)), this, SLOT(setParam(qint8,qint32)));
    QObject::connect(&worker, SIGNAL(getParamRmRq(qint8)), this, SLOT(deleteParam(qint8)));
}

void CanProg::connect(qint8 systemId, qint16 blockId, qint8 modificationBLock)
{
    if ((systemId == this->systemId) && (blockId == this->blockId) && (modificationBLock == this->blockModification))
    {
        emit sendProgStatus(dictionary);
    }
}

void CanProg::getFileList()
{
    QDir dir = QDir::current();
    QStringList files = dir.entryList();
    foreach(QString fileName, files)
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);

        DevFileInfo fileInfo(fileName, file.readAll());
        fileList.append(fileInfo);
    }

    emit sendFileList(fileList);
}

void CanProg::readFile(const QString &fileName, qint32 offset, qint32 readSize)
{
    qint8 errorCode = 0;
    QByteArray buffer;
    if(QFile::exists(fileName))
    {
        QFile file(fileName);

        if (offset+readSize > file.size())
            errorCode = ProgRead::invalidOffset;
        {
            if(file.open(QIODevice::ReadOnly));
            buffer.append(file.read(readSize));
        }
    }
    else
    {
        errorCode = ProgRead::fileNotFound;
    }
    emit sendFile(errorCode, buffer);
}

void CanProg::deleteFile(const QString &fileName)
{
    qint8 errorCode = 0;
    if(QFile::exists(fileName))
    {
        QFile(fileName).remove();
    }
    else
    {
        errorCode = ProgRmAck::FileNotExists;
    }
    emit sendDeleteFileAck(errorCode);
}

void CanProg::deleteAllFiles(qint32 securityKey)
{
    QDir dir = QDir::current();
    QStringList files = dir.entryList();
    foreach(QString fileName, files)
    {
        QFile(fileName).remove();
    }
    emit sendDeleteAllFilesAck();
}

void CanProg::createFile(const QString &fileName, qint32 fileSize)
{
    qint8 errorCode = 0;
    if(!QFile::exists(fileName))
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        file.resize(fileSize);
    }
    else
        errorCode = ProgCreateAck::FileAlreadyExists;
    emit sendCreateFileAck(errorCode);
}

void CanProg::writeFile(const QString &fileName, qint32 offset, const QByteArray &data)
{
    if(!QFile::exists(fileName))
    {
        QFile file(fileName);
        if(file.open(QIODevice::Append))
            file.write(data);
    }
}

void CanProg::setParam(qint8 key, qint32 value)
{

}

void CanProg::deleteParam(qint8 key)
{

}
