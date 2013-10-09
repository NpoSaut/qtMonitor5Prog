#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include "Can/CanNik/workingwithmessage.h"
#include "debugging.h"
#include "Can/can.h"
#include "IsoTp/tpsendtransaction.h"
#include "IsoTp/tpreceivetransaction.h"
#include "Fudp/FudpMessage/message.h"
#include "Fudp/FudpMessage/proginit.h"
#include "Fudp/FudpMessage/progstatus.h"
#include "Fudp/FudpMessage/message.h"
#include "Fudp/canprog.h"

using namespace CanInternals;
using namespace IsoTpFrames;
using namespace IsoTp;
using namespace FudpMessage;
using namespace Fudp;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QHash<qint8, qint32> dict;
    dict[1] = 1;
    dict[2] = 2;
    DeviceTickets tick = {1,2,3,4,5};
    CanProg cp(dict,tick);
    return a.exec();
}
