#include "canprog.h"
#include <QColor>
#include <io.h>

// HACK
// Кастыль!!
#include "Can/CanNik/workingwithmessage.h"

namespace Fudp
{
CanProg::CanProg(PropStore *pStore, QObject *parent) :
    pStore(pStore), QObject(parent), worker(FuInit, FuDev, FuProg), myTicket(), initWaitTimer(), monitor()
{
    progMode = false;
    isSerialNumber = false;

    pStore->get(129, myTicket.blockId);
    pStore->get(130, myTicket.module);
    pStore->get(131, myTicket.blockSerialNumber);
    pStore->get(133, myTicket.channel);
    pStore->get(134, myTicket.modification);

    QDir::setCurrent("C:/MonMSUL/root");

    QObject::connect(this, SIGNAL(sendAnswerToBroadcast(DeviceTickets)), &worker, SLOT(sendAnswerToBroadcast(DeviceTickets)));
    QObject::connect(this, SIGNAL(sendProgStatus(QVector<QPair<quint8,qint32> >)), &worker, SLOT(sendProgStatus(QVector<QPair<quint8,qint32> >)));
    QObject::connect(this, SIGNAL(sendFileList(QMap<QString, DevFileInfo>)), &worker, SLOT(sendProgList(QMap<QString, DevFileInfo>)));
    QObject::connect(this, SIGNAL(sendFile(qint8,QByteArray)), &worker, SLOT(sendProgRead(qint8,QByteArray)));
    QObject::connect(this, SIGNAL(sendDeleteFileAck(qint8)), &worker, SLOT(sendProgRmAck(qint8)));
    QObject::connect(this, SIGNAL(sendDeleteAllFilesAck()), &worker, SLOT(sendProgMrPropperAck()));
    QObject::connect(this, SIGNAL(sendCreateFileAck(qint8)), &worker, SLOT(sendProgCreateAck(qint8)));
    QObject::connect(this, SIGNAL(sendWriteFileAck(qint8)), &worker, SLOT(sendProgWriteAck(qint8)));
    QObject::connect(this, SIGNAL(sendSetParamAck(qint8)), &worker, SLOT(sendParamSetAck(qint8)));
    QObject::connect(this, SIGNAL(sendDeleteParamAck(qint8)), &worker, SLOT(sendParamRmAck(qint8)));
    QObject::connect(this, SIGNAL(sendFirmCorrupt()), &worker, SLOT(sendProgFirmCorrupt()));
    QObject::connect(this, SIGNAL(sendSubmitAck(qint8)), &worker, SLOT(sendSubmitAck(qint8)));

    QObject::connect(&worker, SIGNAL(getProgInit(DeviceTickets)), this, SLOT(connect(DeviceTickets)));
    QObject::connect(&worker, SIGNAL(getProgListRq()), this, SLOT(getFileList()));
    QObject::connect(&worker, SIGNAL(getProgReadRq(QString,qint32,qint32)), this, SLOT(readFile(QString,qint32,qint32)));
    QObject::connect(&worker, SIGNAL(getProgRm(QString)), this, SLOT(deleteFile(QString)));
    QObject::connect(&worker, SIGNAL(getProgMrPropper(qint32)), this, SLOT(deleteAllFiles(qint32)));
    QObject::connect(&worker, SIGNAL(getProgCreate(QString,qint32)), this, SLOT(createFile(QString,qint32)));
    QObject::connect(&worker, SIGNAL(getProgWrite(QString,qint32,QByteArray)), this, SLOT(writeFile(QString,qint32,QByteArray)));
    QObject::connect(&worker, SIGNAL(getParamSetRq(qint8,qint32)), this, SLOT(setParam(qint8,qint32)));
    QObject::connect(&worker, SIGNAL(getParamRmRq(qint8)), this, SLOT(deleteParam(qint8)));
    QObject::connect(&worker, SIGNAL(getProgSubmit(qint8)), this, SLOT(submit(qint8)));
    QObject::connect(&worker, SIGNAL(waitingTimeOut()), this, SLOT(timeOut()));

    QObject::connect(&initWaitTimer, SIGNAL(timeout()), this, SLOT(periodicalCheck()));


    QObject::connect(&monitor, SIGNAL(finished(int)), this, SLOT(start(int)));

    initWaitTimer.setInterval(1000);
    initWaitTimer.setSingleShot(true);
//    initWaitTimer.start();

    if(myTicket.blockSerialNumber != 0)
        isSerialNumber = true;
        takeFileList();
    }
}

void CanProg::connect(const DeviceTickets &tickets)
{
    if(isSerialNumber)
        if (myTicket == tickets)
        {
            initWaitTimer.stop();

            progMode = true;

            emit sendProgStatus(pStore->data());
            emit sendState(tr("Идет прошивка"));
        }
        else if (myTicket <= tickets) // Броадкаст
        {
            LOG_WRITER.write(tr("Получен броадкаст"), QColor(0, 255, 0));
            emit sendAnswerToBroadcast(myTicket);
        }
        else
        {
            progModeExit();
        }
}

void CanProg::getFileList()
{
    if(progMode)
    {
        LOG_WRITER.write(tr("Запрос списка файлов"), QColor(0, 255, 0));
        takeFileList();
        emit sendFileList(fileList);
    }
}

void CanProg::readFile(const QString &fileName, qint32 offset, qint32 readSize)
{
    if(progMode)
    {
        qint8 errorCode = 0;
        QByteArray buffer;
        if(fileList.contains(fileName))
        {
            if (offset+readSize > fileList[fileName].getFileSize())
                errorCode = ProgRead::invalidOffset;
            else
                buffer.append(fileList[fileName].getData(offset, readSize));
        }
        else
        {
            errorCode = ProgRead::fileNotFound;
        }
        emit sendFile(errorCode, buffer);
    }
}

void CanProg::deleteFile(const QString &fileName)
{
    if(progMode)
    {
        qint8 errorCode = 0;
        if(fileList.contains(fileName))
        {
            LOG_WRITER.write(QString(tr("Удаление файла %1")).arg(fileName), QColor(0, 255, 0));
            fileList.remove(fileName);
        }
        else
            errorCode = ProgRmAck::FileNotExists;

        emit sendDeleteFileAck(errorCode);
    }
}

void CanProg::deleteAllFiles(qint32 securityKey)
{
    if(progMode)
    {
        fileList.clear();
//        QDir dir = QDir::current();
//        QStringList files = dir.entryList();
//        foreach(QString fileName, files)
//        {
//            QFile(fileName).remove();
//        }

        emit sendDeleteAllFilesAck();
    }

}

void CanProg::createFile(const QString &fileName, qint32 fileSize)
{
    if(progMode)
    {

        qint8 errorCode = 0;
        if(!fileList.contains(fileName))
        {
            bool success = true;
            int lastSlashPosition = fileName.lastIndexOf('/');
            if (lastSlashPosition != -1)
                success &= QDir::current().mkpath(fileName.left(lastSlashPosition));
//            QFile file(fileName);
//            success &= file.open(QIODevice::WriteOnly);
//            file.resize(fileSize);
//            file.close();
            DevFileInfo fileInfo(fileSize);
            fileList[fileName] = fileInfo;
            errorCode = success ? 0 : ProgCreateAck::ErrorCreate;
        }
        else
            errorCode = ProgCreateAck::FileAlreadyExists;

        emit sendCreateFileAck(errorCode);
    }
}

void CanProg::writeFile(const QString &fileName, qint32 offset, const QByteArray &data)
{
    if(progMode)
    {
        qint8 errorCode = 0;
        if(fileList.contains(fileName))
        {
//            QFile file(fileName);
//            if(file.open(QIODevice::ReadWrite))
//            {
//                file.seek(offset);
//                if(file.write(data) == -1)54

//                    errorCode = 255;
//            }
//            file.close();
            LOG_WRITER.write(QString(tr("Запись в файл %1")).arg(fileName), QColor(0, 255, 0));
            if(!fileList[fileName].setData(data, offset))
                errorCode = 1;
        }
        emit sendWriteFileAck(errorCode);
    }
}

void CanProg::setParam(qint8 key, qint32 value)
{
    if(progMode)
        emit sendSetParamAck( pStore->set(key, value) ? 0 : 3 );
}

void CanProg::deleteParam(qint8 key)
{
    if(progMode)
        emit sendDeleteParamAck( pStore->del(key) ? 0 : 2 );
}

void CanProg::submit(qint8 submitKey)
{
    if(submitKey == 0)
    {
        periodicalCheck();
    }
    else
    {
        int errorCode = 2;
        takeFileList();

        if(!pStore->get(129, myTicket.blockId))
            errorCode = 3;
        if(!pStore->get(130, myTicket.module))
            errorCode = 3;
        if(!pStore->get(131, myTicket.blockSerialNumber))
            errorCode = 3;
        if(!pStore->get(133, myTicket.channel))
            errorCode = 3;
        if(!pStore->get(134, myTicket.modification))
            errorCode = 3;
        emit sendSubmitAck(errorCode);
        periodicalCheck();
    }
}

void CanProg::periodicalCheck()
{
    if(isSerialNumber)
    {
        int errorCode = 0;
        if(progMode)
            LOG_WRITER.write(tr("Проверка целостности прошивки"), QColor(0, 255, 0));

        if (checkProgram())
        {
            if(!pStore->sync() || !saveChanges())
                errorCode = 1;
            progModeExit(errorCode);
        }
        else
        {
            emit sendFirmCorrupt();
            emit sendState(tr("Прошивка повреждена"));
            initWaitTimer.start();
        }
    }
    else
        emit noSerialNumber();
}

QStringList CanProg::parseDir(const QDir dir)
{
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

void CanProg::progModeExit(int errorCode)
{
    if(progMode)
    {
        emit sendState(tr(""));    
        emit sendSubmitAck(errorCode);
    }
    progMode = false;
    emit sendState(tr(""));
    CanInternals::canDrv.stop();
    QDir::setCurrent("C:/");
    monitor.start("MonMSUL/root/Monitor.exe");
}

bool CanProg::checkProgram()
{
    if(progMode)
        emit sendState(tr("Проверка целостности прошивки..."));

    auto files = fileList.keys();

    quint16 crc = 0;
    foreach(QString fileName, files)
    {
//        QFile file(fileName);
//        file.open(QIODevice::ReadOnly);

//        DevFileInfo fileInfo(/*fileName.remove(0,2), */file.readAll());
        crc ^= fileList[fileName].getControlSum();
    }
    qint32 etalonCrc;
    if ( pStore->get(6, etalonCrc) )
        return crc == etalonCrc;
    else
        return false;
}

void CanProg::inputBlockSerialNumber(qint32 blockSerialNumber)
{
    pStore->set(131, blockSerialNumber);
    pStore->sync();
    myTicket.blockSerialNumber = blockSerialNumber;
    isSerialNumber = true;
    initWaitTimer.start();
}

void CanProg::start(int exitCode)
{
    QDir::setCurrent("C:/MonMSUL/root");
    CanInternals::canDrv.start();
    initWaitTimer.start();
}

bool CanProg::saveChanges()
{
    bool success = true;
    auto keys = fileList.keys();
    QDir rootDir ("C:/MonMSUL/root"); // TODO: Внимание! Захардкоженый путь!!!
    QStringList deleteList = parseDir(rootDir);
    foreach(QString name, deleteList)
        QFile(name).remove();
    rootDir.mkpath(".");

    foreach(QString key, keys)
    {
        QFile file(key);
        if(file.open(QIODevice::ReadWrite))
        {
            if(file.write(fileList[key].getData()) == -1)
                success = false;
            file.flush();
            _commit(file.handle());
            file.close();
        }
        else
            success = false;
    }
    return success;
}

void CanProg::takeFileList()
{
    QDir dir = QDir(".");
    QStringList files = parseDir(dir);
    fileList.clear();
    foreach(QString fileName, files)
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);

        DevFileInfo fileInfo(/*fileName.remove(0,2), */file.readAll());
        fileList[fileName] = fileInfo;

        file.close();
    }
}

void CanProg::timeOut()
{
    takeFileList();
    periodicalCheck();
}

void CanProg::drvStart()
{
    canDrv.start();
    initWaitTimer.start();
}

}
