#include "canprog.h"

namespace Fudp
{
CanProg::CanProg(PropStore *pStore, QObject *parent) :
    pStore(pStore), QObject(parent), worker(FuDev, FuInit), myTicket()
{
    pStore->get(129, myTicket.blockId);
    pStore->get(130, myTicket.module);
    pStore->get(131, myTicket.blockSerialNumber);
    pStore->get(133, myTicket.channel);
    pStore->get(134, myTicket.modification);

    QDir::setCurrent("./root");

    QObject::connect(this, SIGNAL(sendAnswerToBroadcast(DeviceTickets)), &worker, SLOT(sendAnswerToBroadcast(DeviceTickets)));
    QObject::connect(this, SIGNAL(sendProgStatus(QVector<QPair<quint8,qint32> >)), &worker, SLOT(sendProgStatus(QVector<QPair<quint8,qint32> >)));
    QObject::connect(this, SIGNAL(sendFileList(QList<DevFileInfo>)), &worker, SLOT(sendProgList(QList<DevFileInfo>)));
    QObject::connect(this, SIGNAL(sendFile(qint8,QByteArray)), &worker, SLOT(sendProgRead(qint8,QByteArray)));
    QObject::connect(this, SIGNAL(sendDeleteFileAck(qint8)), &worker, SLOT(sendProgRmAck(qint8)));
    QObject::connect(this, SIGNAL(sendDeleteAllFilesAck()), &worker, SLOT(sendProgMrPropperAck()));
    QObject::connect(this, SIGNAL(sendCreateFileAck(qint8)), &worker, SLOT(sendProgCreateAck(qint8)));
    QObject::connect(this, SIGNAL(sendSetParamAck(qint8)), &worker, SLOT(sendParamSetAck(qint8)));
    QObject::connect(this, SIGNAL(sendDeleteParamAck(qint8)), &worker, SLOT(sendParamRmAck(qint8)));

    QObject::connect(&worker, SIGNAL(getProgInit(DeviceTickets)), this, SLOT(connect(DeviceTickets)));
    QObject::connect(&worker, SIGNAL(getProgListRq()), this, SLOT(getFileList()));
    QObject::connect(&worker, SIGNAL(getProgReadRq(QString,qint32,qint32)), this, SLOT(readFile(QString,qint32,qint32)));
    QObject::connect(&worker, SIGNAL(getProgRm(QString)), this, SLOT(deleteFile(QString)));
    QObject::connect(&worker, SIGNAL(getProgMrPropper(qint32)), this, SLOT(deleteAllFiles(qint32)));
    QObject::connect(&worker, SIGNAL(getProgCreate(QString,qint32)), this, SLOT(createFile(QString,qint32)));
    QObject::connect(&worker, SIGNAL(getProgWrite(QString,qint32,QByteArray)), this, SLOT(writeFile(QString,qint32,QByteArray)));
    QObject::connect(&worker, SIGNAL(getParamSetRq(qint8,qint32)), this, SLOT(setParam(qint8,qint32)));
    QObject::connect(&worker, SIGNAL(getParamRmRq(qint8)), this, SLOT(deleteParam(qint8)));
}

void CanProg::connect(const DeviceTickets &tickets)
{
    if (myTicket == tickets)
    {
        worker.setAcknowlegmentDescriptor(FuProg);
        emit sendProgStatus(pStore->data());
    }
    else if (myTicket <= tickets) // Броадкаст
    {
        emit sendAnswerToBroadcast(myTicket);
    }
    else
    {
        // TODO: Выход из режима программирования
    }
}

void CanProg::getFileList()
{
    QDir dir = QDir(".");
    QStringList files = parseDir(dir);

    fileList.clear();
    foreach(QString fileName, files)
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);

        DevFileInfo fileInfo(fileName.remove(0,2), file.readAll());
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
        QFile(fileName).remove();
    else
        errorCode = ProgRmAck::FileNotExists;

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
        bool success = true;
        int lastSlashPosition = fileName.lastIndexOf('/');
        if (lastSlashPosition != -1)
            success &= QDir::current().mkpath(fileName.left(lastSlashPosition));
        QFile file(fileName);
        success &= file.open(QIODevice::WriteOnly);
        file.resize(fileSize);
        file.close();

        errorCode = success ? 0 : ProgCreateAck::ErrorCreate;
    }
    else
        errorCode = ProgCreateAck::FileAlreadyExists;
    emit sendCreateFileAck(errorCode);
}

void CanProg::writeFile(const QString &fileName, qint32 offset, const QByteArray &data)
{
    if(QFile::exists(fileName))
    {
        QFile file(fileName);
        if(file.open(QIODevice::ReadWrite))
        {
            file.seek(offset);
            file.write(data);
        }
        file.close();
    }
}

void CanProg::setParam(qint8 key, qint32 value)
{

}

void CanProg::deleteParam(qint8 key)
{

}

QStringList CanProg::parseDir(const QDir dir)
{
    QDir currentDir = dir;
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList fileList;
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if (fileInfo.isDir())
            fileList.append(parseDir(fileInfo.filePath()));
        else
            fileList.append(fileInfo.filePath());
    }
    return fileList;
}

}
