#include "canprog.h"
#include <QColor>

// HACK
// Кастыль!!
#include "Can/CanNik/workingwithmessage.h"

namespace Fudp
{
CanProg::CanProg(PropStore *pStore, QObject *parent) :
    pStore(pStore), QObject(parent), worker(FuInit, FuDev, FuProg), myTicket(), initWaitTimer(), monitor(), log()
{
    progMode = false;

    pStore->get(129, myTicket.blockId);
    pStore->get(130, myTicket.module);
    pStore->get(131, myTicket.blockSerialNumber);
    pStore->get(133, myTicket.channel);
    pStore->get(134, myTicket.modification);

    QDir::setCurrent("C:/MonMSUL/root");

    QObject::connect(this, SIGNAL(sendAnswerToBroadcast(DeviceTickets)), &worker, SLOT(sendAnswerToBroadcast(DeviceTickets)));
    QObject::connect(this, SIGNAL(sendProgStatus(QVector<QPair<quint8,qint32> >)), &worker, SLOT(sendProgStatus(QVector<QPair<quint8,qint32> >)));
    QObject::connect(this, SIGNAL(sendFileList(QList<DevFileInfo>)), &worker, SLOT(sendProgList(QList<DevFileInfo>)));
    QObject::connect(this, SIGNAL(sendFile(qint8,QByteArray)), &worker, SLOT(sendProgRead(qint8,QByteArray)));
    QObject::connect(this, SIGNAL(sendDeleteFileAck(qint8)), &worker, SLOT(sendProgRmAck(qint8)));
    QObject::connect(this, SIGNAL(sendDeleteAllFilesAck()), &worker, SLOT(sendProgMrPropperAck()));
    QObject::connect(this, SIGNAL(sendCreateFileAck(qint8)), &worker, SLOT(sendProgCreateAck(qint8)));
    QObject::connect(this, SIGNAL(sendSetParamAck(qint8)), &worker, SLOT(sendParamSetAck(qint8)));
    QObject::connect(this, SIGNAL(sendDeleteParamAck(qint8)), &worker, SLOT(sendParamRmAck(qint8)));
    QObject::connect(this, SIGNAL(sendFirmCorrupt()), &worker, SLOT(sendProgFirmCorrupt()));

    QObject::connect(&worker, SIGNAL(getProgInit(DeviceTickets)), this, SLOT(connect(DeviceTickets)));
    QObject::connect(&worker, SIGNAL(getProgListRq()), this, SLOT(getFileList()));
    QObject::connect(&worker, SIGNAL(getProgReadRq(QString,qint32,qint32)), this, SLOT(readFile(QString,qint32,qint32)));
    QObject::connect(&worker, SIGNAL(getProgRm(QString)), this, SLOT(deleteFile(QString)));
    QObject::connect(&worker, SIGNAL(getProgMrPropper(qint32)), this, SLOT(deleteAllFiles(qint32)));
    QObject::connect(&worker, SIGNAL(getProgCreate(QString,qint32)), this, SLOT(createFile(QString,qint32)));
    QObject::connect(&worker, SIGNAL(getProgWrite(QString,qint32,QByteArray)), this, SLOT(writeFile(QString,qint32,QByteArray)));
    QObject::connect(&worker, SIGNAL(getParamSetRq(qint8,qint32)), this, SLOT(setParam(qint8,qint32)));
    QObject::connect(&worker, SIGNAL(getParamRmRq(qint8)), this, SLOT(deleteParam(qint8)));
    QObject::connect(&worker, SIGNAL(getProgSubmit()), this, SLOT(submit()));
    QObject::connect(&worker, SIGNAL(waitingTimeOut()), this, SLOT(periodicalCheck()));

    QObject::connect(&initWaitTimer, SIGNAL(timeout()), this, SLOT(periodicalCheck()));


    QObject::connect(&monitor, SIGNAL(finished(int)), this, SLOT(start(int)));

    initWaitTimer.setInterval(1000);
    initWaitTimer.setSingleShot(true);
    initWaitTimer.start();

    LOG_WRITER.installLog();
}

void CanProg::connect(const DeviceTickets &tickets)
{
        if (myTicket == tickets)
        {
            LOG_WRITER.write(tr("Установлено соединение"), QColor(0, 255, 0));

            initWaitTimer.stop();

            LOG_WRITER.write(tr("Отправка списка свойств"), QColor(0, 255, 0));

            progMode = true;

            emit sendProgStatus(pStore->data());
            emit sendState(tr("Идет прошивка"));
        }
        else if (myTicket <= tickets) // Броадкаст
        {
            LOG_WRITER.write(tr("Получен броадкаст"), QColor(0, 255, 0));
            LOG_WRITER.write(tr("Отправка ответа"), QColor(0, 255, 0));

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

        LOG_WRITER.write(tr("Отправка списка файлов"), QColor(0, 255, 0));
        emit sendFileList(fileList);
    }
}

void CanProg::readFile(const QString &fileName, qint32 offset, qint32 readSize)
{
    if(progMode)
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
        LOG_WRITER.write(tr("Отправка ") + fileName, QColor(0, 255, 0));
        emit sendFile(errorCode, buffer);
    }
}

void CanProg::deleteFile(const QString &fileName)
{
    if(progMode)
    {
        LOG_WRITER.write(QString(tr("Удаление файла %1")).arg(fileName), QColor(0, 255, 0));
        qint8 errorCode = 0;
        if(QFile::exists(fileName))
            QFile(fileName).remove();
        else
            errorCode = ProgRmAck::FileNotExists;

        LOG_WRITER.write(QString(tr("Отправка подтверждения удаления файла %1")).arg(fileName), QColor(0, 255, 0));
        emit sendDeleteFileAck(errorCode);
    }
}

void CanProg::deleteAllFiles(qint32 securityKey)
{
    if(progMode)
    {
        LOG_WRITER.write(tr("Удаление всех файлов"), QColor(0, 255, 0));
        QDir dir = QDir::current();
        QStringList files = dir.entryList();
        foreach(QString fileName, files)
        {
            QFile(fileName).remove();
        }

        LOG_WRITER.write(tr("Отправка подверждения удаления"), QColor(0, 255, 0));
        emit sendDeleteAllFilesAck();
    }

}

void CanProg::createFile(const QString &fileName, qint32 fileSize)
{
    if(progMode)
    {
        LOG_WRITER.write(QString(tr("Создание файла %1, размером %2")).arg(fileName).arg(fileSize), QColor(0, 255, 0));

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
            LOG_WRITER.write(QString(tr("Отправка подтверждения создания файла %1, размером %2")).arg(file.fileName()).arg(file.size()), QColor(0, 255, 0));
        }
        else
        {
            LOG_WRITER.write(tr("Отправка сообщение об ошибке"), QColor(0, 255, 0));
            errorCode = ProgCreateAck::FileAlreadyExists;
        }
        emit sendCreateFileAck(errorCode);
    }
}

void CanProg::writeFile(const QString &fileName, qint32 offset, const QByteArray &data)
{
    if(progMode)
    {
        LOG_WRITER.write(QString(tr("Запись в файл %1 блока данных размером %2 байт")).arg(fileName).arg(data.size()), QColor(0, 255, 0));

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

void CanProg::submit()
{
    periodicalCheck();
}

void CanProg::periodicalCheck()
{
    if(progMode)
        LOG_WRITER.write(tr("Проверка целостности прошивки"), QColor(0, 255, 0));

    if (checkProgram())
        progModeExit();
    else
    {
        LOG_WRITER.write(tr("Не сошлась контрольная сумма"), QColor(255, 0, 0));

        emit sendFirmCorrupt();
        emit sendState(tr("Прошивка повреждена"));
        initWaitTimer.start();
    }
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

void CanProg::progModeExit()
{
    progMode = false;
    emit sendState(tr(""));
    LOG_WRITER.finishLog();
    CanInternals::canDrv.stop();
    QDir::setCurrent("C:/");
    monitor.start("MonMSUL/root/Monitor.exe");
}

bool CanProg::checkProgram()
{

    emit sendState(tr("Проверка целостности прошивки..."));
    QDir dir = QDir(".");
    QStringList files = parseDir(dir);

    quint16 crc = 0;
    foreach(QString fileName, files)
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);

        DevFileInfo fileInfo(fileName.remove(0,2), file.readAll());
        crc ^= fileInfo.getControlSum();
    }
    qint32 etalonCrc;
    if ( pStore->get(6, etalonCrc) )
         return crc == etalonCrc;
    else
        return false;
}

void CanProg::start(int exitCode)
{
    LOG_WRITER.installLog();
    QDir::setCurrent("C:/MonMSUL/root");
    CanInternals::canDrv.start();
    initWaitTimer.start();
}

}
