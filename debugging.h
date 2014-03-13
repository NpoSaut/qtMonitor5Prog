#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <QObject>
#include <QFile>
#include "Fudp/devfileinfo.h"
#include "Fudp/FudpMessage/message.h"
#include "IsoTp/isotpcommunicator.h"

using namespace IsoTpFrames;
using namespace IsoTp;
using namespace FudpMessage;

class Debugging : public QObject
{
    Q_OBJECT
public:
    explicit Debugging(Can *can, int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent = 0);
    void send(const QByteArray &arr);
private:    
    IsoTpCommunicator it;

signals:


public slots:
    void createFile(const std::vector<byte> &data);
};

class CanLogger : public QObject
{
    Q_OBJECT
public:
    explicit CanLogger(Can *can, QObject *parent = 0);

public slots:
    void logMessage (CanFrame frame);

private:
    quint32 prev;
};

#endif // DEBUGGING_H
