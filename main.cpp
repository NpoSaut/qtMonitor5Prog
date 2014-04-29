#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QProcess>

#include "canprogworker.h"

// Драйвер CAN
Can *can;
#ifdef LIB_CAN_NICK
#include "qtCanLib/cannick.h"
#elif defined(LIB_SOCKET_CAN)
#include "qtCanLib/socketcan.h"
#else
#include "qtCanLib/dummycan.h"
#endif

#include "qtBlokLib/parser.h"

using namespace IsoTpFrames;
using namespace IsoTp;
using namespace Fudp;

CanProgWorker *canProgWorker;
Parser *blokMessage;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    #ifdef LIB_CAN_NICK
    can = new CanNick ();
    #elif defined(LIB_SOCKET_CAN)
    can = new SocketCan ();
    #else
    can = new DummyCan ();
    #endif

    blokMessage = new Parser (can);

        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

        qDebug() << "PROFILAKTIKA";

    if (a.arguments ().size () >= 3)
    {
        canProgWorker = new CanProgWorker (can, blokMessage, a.arguments ().at(1), a.arguments ().at (2), a.arguments ().at (3));
        return a.exec();
    }
    else
    {
        qDebug() << "Thre arguments needed: ";
        qDebug() << "first - root directory";
        qDebug() << "second - path to prop.txt for hardware";
        qDebug() << "third - path to prop.txt for software module";
        qDebug() << "This is all.";
        return 1;
    }

}
