#include "canprog.h"

#include "PropStore/constloaderstore.h"

namespace Fudp
{
CanProg::CanProg(Can *can, PropStore *hwStore, PropStore *pStore, QDir rootDir, QObject *parent) :
    QObject(parent),
    hwStore(hwStore),
    pStore(pStore),
    worker(can, FuInit, FuDev, FuProg, parent),
    myTicket(),
    rootDir(rootDir),
    sessionActiveTimer(),
    progMode (false),
    isSerialNumberSet (false)
{
    loaderPropStore = new ConstLoaderStore (3, 6, 4);

    sessionActiveTimer.setInterval (10000);

    hwStore->get(129, myTicket.blockId);
    hwStore->get(131, myTicket.blockSerialNumber);
    hwStore->get(133, myTicket.channel);
    hwStore->get(134, myTicket.modification);

    pStore->get(130, myTicket.module);

    QDir::setCurrent( rootDir.absolutePath () );

    QObject::connect(this, SIGNAL(sendAnswerToBroadcast(DeviceTicket)), &worker, SLOT(sendAnswerToBroadcast(DeviceTicket)));
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
    QObject::connect(this, SIGNAL(sendPong(quint8,ProgPong::Status)), &worker, SLOT(sendProgPong(quint8,ProgPong::Status)));

    QObject::connect(&worker, SIGNAL(getProgInit(DeviceTicket)), this, SLOT(connect(DeviceTicket)));
    QObject::connect(&worker, SIGNAL(getProgListRq()), this, SLOT(getFileList()));
    QObject::connect(&worker, SIGNAL(getProgReadRq(QString,qint32,qint32)), this, SLOT(readFile(QString,qint32,qint32)));
    QObject::connect(&worker, SIGNAL(getProgRm(QString)), this, SLOT(deleteFile(QString)));
    QObject::connect(&worker, SIGNAL(getProgMrPropper(qint32)), this, SLOT(deleteAllFiles(qint32)));
    QObject::connect(&worker, SIGNAL(getProgCreate(QString,qint32)), this, SLOT(createFile(QString,qint32)));
    QObject::connect(&worker, SIGNAL(getProgWrite(QString,qint32,QByteArray)), this, SLOT(writeFile(QString,qint32,QByteArray)));
    QObject::connect(&worker, SIGNAL(getParamSetRq(qint8,qint32)), this, SLOT(setParam(qint8,qint32)));
    QObject::connect(&worker, SIGNAL(getParamRmRq(qint8)), this, SLOT(deleteParam(qint8)));
    QObject::connect(&worker, SIGNAL(getProgSubmit(qint8)), this, SLOT(submit(qint8)));
    QObject::connect(&this->sessionActiveTimer, SIGNAL(timeout()), this, SLOT(sessionTimeOut()));
    QObject::connect(&worker, SIGNAL(getProgPing(quint8)), this, SLOT(controlSession(quint8)));

    QObject::connect(&worker, SIGNAL(getProgInit(DeviceTicket)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getProgListRq()), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getProgReadRq(QString,qint32,qint32)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getProgRm(QString)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getProgMrPropper(qint32)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getProgCreate(QString,qint32)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getProgWrite(QString,qint32,QByteArray)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getParamSetRq(qint8,qint32)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getParamRmRq(qint8)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getProgSubmit(qint8)), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(waitingTimeOut()), this, SLOT(prolongSession()));
    QObject::connect(&worker, SIGNAL(getProgPing(quint8)), this, SLOT(prolongSession()));

    if(myTicket.blockSerialNumber != 0)
    {
        //LOG_WRITER.installLog();
        isSerialNumberSet = true;
        takeFileList();
    }
}

void CanProg::connect(const DeviceTicket &requestedTicket)
{
    if(isSerialNumberSet)
        if (myTicket == requestedTicket)
        {
            progMode = true;

            emit sendProgStatus(hwStore->data () + pStore->data() + loaderPropStore->data ());

            emit progModeChanged (true);
            emit sendState(tr("Идет прошивка"));

            sessionActiveTimer.start ();
        }
        else if (myTicket <= requestedTicket) // Броадкаст
        {
            //LOG_WRITER.write(tr("Получен броадкаст"), QColor(0, 255, 0));
            emit sendAnswerToBroadcast(myTicket);
        }
        else
        {
            exitProgMode();
        }
}

void CanProg::getFileList()
{
    if(progMode)
    {
        //LOG_WRITER.write(tr("Запрос списка файлов"), QColor(0, 255, 0));
        takeFileList();
        emit sendFileList(fileList);
    }
}

void CanProg::readFile(const QString &fileName, qint32 offset, qint32 readSize)
{
    if(progMode)
    {
        //LOG_WRITER.write(QString(tr("Чтение файла %1")).arg(fileName), QColor(0, 255, 0));
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
            //LOG_WRITER.write(QString(tr("Удаление файла %1")).arg(fileName), QColor(0, 255, 0));
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
        //LOG_WRITER.write(QString(tr("Создание файла %1")).arg(fileName), QColor(0, 255, 0));
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
            //LOG_WRITER.write(QString(tr("Запись в файл %1")).arg(fileName), QColor(0, 255, 0));
            if(!fileList[fileName].setData(data, offset))
                errorCode = 1;
        }
        emit sendWriteFileAck(errorCode);
    }
}

void CanProg::setParam(qint8 key, qint32 value)
{
    if(progMode)
    {
        //LOG_WRITER.write(QString(tr("Запись свойства %1 со значением %2")).arg(key).arg(value), QColor(0, 255, 0));
        if ( isLoaderProperty (key) )
            emit sendSetParamAck( 0 );
        else if ( isHwProperty (key))
            emit sendSetParamAck( hwStore->set(key, value) ? 0 : 3 );
        else
            emit sendSetParamAck( pStore->set(key, value) ? 0 : 3 );
    }
}

void CanProg::deleteParam(qint8 key)
{
    if(progMode)
    {
        //LOG_WRITER.write(QString(tr("Удаление совйства %1")).arg(key), QColor(0, 255, 0));
        if ( isLoaderProperty (key) )
            emit sendDeleteParamAck( 0 );
        else if ( isHwProperty (key) )
            emit sendDeleteParamAck( hwStore->del(key) ? 0 : 2 );
        else
            emit sendDeleteParamAck( pStore->del(key) ? 0 : 2 );
    }
}

void CanProg::submit(qint8 submitKey)
{
    if (progMode)
    {
        int status = 255;

        if(submitKey == 0) // Применить
        {
            //LOG_WRITER.write(tr("Проверка целостности прошивки"), QColor(0, 255, 0));
            if ( checkFirmware() ) // Тестируем, что нам прислали
            {
                if( saveChanges() )
                    status = 0;
                else
                    status = 1;
            }
            else
            {
                //LOG_WRITER.write(tr("Не сошлась контрольная сумма"), QColor(255, 0, 0));
                emit sendFirmCorrupt();
                emit sendState(tr("Прошивка повреждена"));
                status = 1;
            }
        }
        else // Отменить
        {
            discardChanges ();
            status = 2;
        }

        qDebug() << "Submit ack: " << status;
        emit sendSubmitAck(status);
        exitProgMode();
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
            fileList.append(fileInfo.filePath().remove("./"));
    }
    return fileList;
}

bool CanProg::isLoaderProperty(qint8 key)
{
    return key == 192 || key == 193 || key == 194 || key == 195 || key == 196;
}

bool CanProg::isHwProperty(qint8 key)
{
    return key == 129 || key == 131 || key == 132 || key == 133 || key == 134;
}

void CanProg::exitProgMode()
{
    if(progMode)
    {
        progMode = false;
        emit progModeChanged (false);
        sessionActiveTimer.stop ();
    }
    //LOG_WRITER.finishLog();
}

bool CanProg::checkFirmware()
{
    bool ok = false;
    if(progMode)
        emit sendState(tr("Проверка целостности прошивки..."));

    quint16 crc = 0;
    auto files = fileList.keys();
    foreach(QString fileName, files)
        crc ^= fileList[fileName].getControlSum();

    qint32 etalonCrc;
    if ( pStore->get(6, etalonCrc) )
        ok = (crc == etalonCrc);

    emit crcCheckChanged (ok);
    return ok;
}

void CanProg::inputBlockSerialNumber(qint32 blockSerialNumber)
{
    hwStore->set(131, blockSerialNumber);
    pStore->sync();
    if ( hwStore->get (131, myTicket.blockSerialNumber) )
        isSerialNumberSet = true;
    //LOG_WRITER.installLog();
}

bool CanProg::saveChanges()
{
    bool success = pStore->sync() && hwStore->sync ();

    auto keys = fileList.keys();
    QStringList deleteList = parseDir(rootDir);
    foreach(QString name, deleteList)
        QFile(name).remove();
    rootDir.mkpath(".");

    foreach(QString key, keys)
    {
        QFile file("./" + key);
        if(file.open(QIODevice::ReadWrite))
        {
            if(file.write(fileList[key].getData()) == -1)
                success = false;
            file.flush();
#ifdef MONITOR_5
            _commit(file.handle());
#endif
            file.close();
        }
        else
            success = false;
    }

#ifdef ON_LINUX
    QProcess::execute ("chmod -R +x ./");
#endif

    return success;
}

void CanProg::discardChanges()
{
    takeFileList();
    pStore->discard ();
    hwStore->discard ();
}

void CanProg::takeFileList()
{
    QDir::setCurrent (rootDir.absolutePath ());
    QDir workingDir ("./");
    QStringList files = parseDir(workingDir);
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

void CanProg::sessionTimeOut()
{
    discardChanges ();
    emit sendSubmitAck(2);
    exitProgMode ();
}

void CanProg::controlSession(quint8 counter)
{
    if (progMode)
        emit sendPong (counter, ProgPong::ALIVE);
}

void CanProg::prolongSession()
{
    if (progMode)
        sessionActiveTimer.start ();
}

}
