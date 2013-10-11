#include "debugging.h"

Debugging::Debugging(int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent) :
  it(transmitDescriptor, acknowlegmentDescriptor), QObject(parent)
{

    QObject::connect(&it, SIGNAL(bufferReceived(std::vector<byte>)), this, SLOT(createFile(std::vector<byte>)));
}

void Debugging::createFile(const std::vector<byte> &data)
{
    DevFileInfo devFile("SomeText.txt", Message::fromVectorToQByteArray(data));

    QFile file(devFile.getFileName());
    file.open(QIODevice::Append);
    //QDataStream in(&file);

    //in << devFile.getData();
    file.write(devFile.getData());
    file.close();
    qDebug() << "File created";
}

void Debugging::send(const QByteArray &arr)
{
    it.send(Message::fromQByteArrayToVector(arr));
}


CanLogger::CanLogger(QObject *parent)
    : prev (0)
{
    this->connect(&canDev, SIGNAL(messageReceived(CanFrame)), SLOT(logMessage(CanFrame)));
}

void CanLogger::logMessage(CanFrame frame)
{
    if (frame.getId() == 0x14)
    {
        quint32 n;
        if (frame.getData().size() == 1)
           n = frame.getData()[0];
        else if(frame.getData().size() == 2)
           n = (quint32)frame.getData()[0] + (quint32)frame.getData()[1]*256;
        else if(frame.getData().size() == 3)
           n = (quint32)frame.getData()[0] + (quint32)frame.getData()[1]*256 + (quint32)frame.getData()[1]*256*256;
        else if(frame.getData().size() == 4)
           n = (quint32)frame.getData()[0] + (quint32)frame.getData()[1]*256 + (quint32)frame.getData()[1]*256*256 + (quint32)frame.getData()[1]*256*256*256;

        if (n != prev+1)
            //qDebug() << n;

        prev = n;
    }
}
