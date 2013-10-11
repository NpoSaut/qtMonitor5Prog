#include "workingwithfudpmessage.h"

namespace FudpMessage
{
WorkingWithFudpMessage::WorkingWithFudpMessage(int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent) :
    QObject(parent), communicator(transmitDescriptor, acknowlegmentDescriptor)
{
    QObject::connect(&communicator, SIGNAL(bufferReceived(std::vector<byte>)), this, SLOT(receiveData(std::vector<byte>)));
}

void WorkingWithFudpMessage::receiveData(const std::vector<byte> &data)
{
    switch(data.at(0))
    {
    case MessageId(progInit):
    {
        ProgInit init;
        init.decode(data);
        DeviceTickets tickets = init.getTicket();
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
    }
}

void WorkingWithFudpMessage::sendProgStatus(const QHash<qint8, qint32> &dictionary)
{
    ProgStatus status(dictionary);
    communicator.send(status.encode());
}

void WorkingWithFudpMessage::sendProgList(const QList<DevFileInfo> &list)
{
    ProgList pList(list);
    communicator.send(pList.encode());
}

void WorkingWithFudpMessage::sendProgRead(qint8 errorCode, const QByteArray &data)
{
    ProgRead read(data, errorCode);
    communicator.send(read.encode());
}

void WorkingWithFudpMessage::sendProgRmAck(qint8 errorCode)
{
    ProgRmAck rmAck(errorCode);
    communicator.send(rmAck.encode());
}

void WorkingWithFudpMessage::sendProgMrPropperAck()
{
    ProgMrProper mrPropper;
    communicator.send(mrPropper.encode());
}

void WorkingWithFudpMessage::sendProgCreateAck(qint8 errorCode)
{
    ProgCreateAck createAck(errorCode);
    communicator.send(createAck.encode());
}

void WorkingWithFudpMessage::sendParamRmAck(qint8 errorCode)
{
    ParamRmAck rmParam(errorCode);
    communicator.send(rmParam.encode());
}

void WorkingWithFudpMessage::sendParamSetAck(qint8 errorCode)
{
    ParamSetAck setParam(errorCode);
    communicator.send(setParam.encode());
}
void WorkingWithFudpMessage::setAcknowlegmentDescriptor(int acknowlegmentDescriptor)
{
    communicator.setAcknowlegmentDescriptor(acknowlegmentDescriptor);
}
}
