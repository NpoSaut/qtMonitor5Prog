#include "workingwithfudpmessage.h"
#include "Log/logwriter.h"
namespace FudpMessage
{
WorkingWithFudpMessage::WorkingWithFudpMessage(Can *can, int initDescriptor, int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent) :
    QObject(parent),
    communicator1(can, transmitDescriptor, initDescriptor, parent),
    communicator2(can, transmitDescriptor, acknowlegmentDescriptor, parent),
    count(0)
{
    QObject::connect(&communicator1, SIGNAL(bufferReceived(std::vector<byte>)), this, SLOT(receiveData(std::vector<byte>)));
    QObject::connect(&communicator2, SIGNAL(bufferReceived(std::vector<byte>)), this, SLOT(receiveData(std::vector<byte>)));
    QObject::connect(&communicator2, SIGNAL(waitingTimeOut()), this, SLOT(timeOut()));
}

void WorkingWithFudpMessage::receiveData(const std::vector<byte> &data)
{    
    switch(data.at(0))
    {
    case MessageId(progInit):
    {
        ProgInit init;
        init.decode(data);
        DeviceTicket tickets = init.getTicket();
        emit getProgInit((tickets));
        break;
    }
    case MessageId(progListRq):
    {
        emit getProgListRq();
        break;
    }
    case MessageId(progReadRq):
    {
        ProgReadRq read;
        read.decode(data);
        emit getProgReadRq(read.getFileName(), read.getOffset(), read.getReadSize());
        break;
    }
    case MessageId(progRm):
    {
        ProgRm rm;
        rm.decode(data);
        emit getProgRm(rm.getFileName());
        break;
    }
    case MessageId(progMrProper):
    {
        emit getProgMrPropper(0x123);
        break;
    }
    case MessageId(progCreate):
    {
        ProgCreate create;
        create.decode(data);
        emit getProgCreate(create.getFileName(), create.getFileSize());
        break;
    }
    case MessageId(progWrite):
    {
        ProgWrite write;
        write.decode(data);
        emit getProgWrite(write.getFileName(), write.getOffset(), write.getData());
        break;
    }
    case MessageId(paramSetRq):
    {
        ParamSetRq setParam;
        setParam.decode(data);
        emit getParamSetRq(setParam.getKey(), setParam.getParamValue());
        break;
    }
    case MessageId(paramRmRq):
    {
        ParamRmRq rmParam;
        rmParam.decode(data);
        emit getParamRmRq(rmParam.getKey());
        break;
    }
    case MessageId(progSubmit):
    {
        ProgSubmit sb;
        sb.decode(data);
        emit getProgSubmit(sb.applyCanges());
        break;
    }
    }
}

void WorkingWithFudpMessage::sendAnswerToBroadcast(DeviceTicket ticket)
{
    ProgBroadcastAnswer progBcAnsw(ticket);
    communicator2.send(progBcAnsw.encode());
}

void WorkingWithFudpMessage::sendProgStatus(const QVector<QPair<quint8, qint32> > dictionary)
{
    ProgStatus status(dictionary);
    communicator2.send(status.encode());
}

void WorkingWithFudpMessage::sendProgList(const QMap<QString, DevFileInfo> &list)
{
    ProgList pList(list);
    communicator2.send(pList.encode());
}

void WorkingWithFudpMessage::sendProgRead(qint8 errorCode, const QByteArray &data)
{
    ProgRead read(data, errorCode);
    communicator2.send(read.encode());
}

void WorkingWithFudpMessage::sendProgRmAck(qint8 errorCode)
{
    ProgRmAck rmAck(errorCode);
    communicator2.send(rmAck.encode());
}

void WorkingWithFudpMessage::sendProgMrPropperAck()
{
    ProgMrProper mrPropper;
    communicator2.send(mrPropper.encode());
}

void WorkingWithFudpMessage::sendProgCreateAck(qint8 errorCode)
{
    ProgCreateAck createAck(errorCode);
    communicator2.send(createAck.encode());
}

void WorkingWithFudpMessage::sendProgWriteAck(qint8 errorCode)
{
    ProgWriteAck writeAck(errorCode);
    communicator2.send(writeAck.encode());
}

void WorkingWithFudpMessage::sendParamRmAck(qint8 errorCode)
{
    ParamRmAck rmParam(errorCode);
    communicator2.send(rmParam.encode());
}

void WorkingWithFudpMessage::sendProgFirmCorrupt()
{
    ProgFirmCorrupt fc;
    communicator2.send(fc.encode());
}

void WorkingWithFudpMessage::sendParamSetAck(qint8 errorCode)
{
    ParamSetAck setParam(errorCode);
    communicator2.send(setParam.encode());
}

void WorkingWithFudpMessage::sendSubmitAck(qint8 finalCode)
{
    ProgSubmitAck submitAck(finalCode);
    communicator2.send(submitAck.encode());
}

void WorkingWithFudpMessage::timeOut()
{
    emit waitingTimeOut();
}

}
