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
