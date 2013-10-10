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
using namespace Fudp;

CanLogger logger;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QHash<qint8, qint32> dict;
    dict[1] = 1;
    dict[2] = 2;
    DeviceTickets tic = {1, 2, 3, 4, 5};
//    std::vector<byte> data;
//    for(int i = 0; i < 4095; i++)
//        data.insert(data.end(), i);

//    QFile file("SomeText.txt");
//    Debugging deb(0x4008, 0x3008);

//            TpSendTransaction send(0x4008, 0x3008);
//    IsoTpCommunicator itc(0x4008, 0x3008);
//    if(file.open(QIODevice::ReadOnly))
//    {
//        QByteArray arr(file.readAll());
//        //send.send(Message::fromQByteArrayToVector(arr));
//        itc.send(Message::fromQByteArrayToVector(arr));
//    }
//    else
//        qDebug() << "File doesn't open";

    //deb.send();
    //    QByteArray arr;
    //    QDataStream in(&arr, QIODevice::ReadWrite);
    //    in.setByteOrder(QDataStream::LittleEndian);
    //    in << 329;
    //    for(int i = 0; i < arr.size(); i++)
    //        qDebug("%x", arr.at(i));
    CanProg cp(dict, tic);
    return a.exec();
}
