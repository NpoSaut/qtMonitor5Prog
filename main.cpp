#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QProcess>
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
#include "PropStore/simplefilepropstore.h"

using namespace CanInternals;
using namespace IsoTpFrames;
using namespace IsoTp;
using namespace Fudp;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DeviceTickets tic = {3, 1488, 1, 1, 1};
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
    QFile pFile("C:/MonMSUL/prop.txt");
    SimpleFilePropStore pStore(pFile);
    CanProg cp(&pStore);

    return a.exec();
}
