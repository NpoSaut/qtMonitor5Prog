#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <QObject>
#include <QFile>
#include "Fudp/devfileinfo.h"
#include "Fudp/FudpMessage/message.h"
#include "IsoTp/isotpcommunicator.h"

using namespace FudpMessage;
using namespace CanInternals;
using namespace IsoTpFrames;
using namespace IsoTp;

class Debugging : public QObject
{
    Q_OBJECT
public:
    explicit Debugging(int transmitDescriptor, int acknowlegmentDescriptor, QObject *parent = 0);
    void send(const QByteArray &arr);
private:    
    IsoTpCommunicator it;

signals:


public slots:
    void createFile(const std::vector<byte> &data);
};

#endif // DEBUGGING_H
