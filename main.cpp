#include <QCoreApplication>
#include <QApplication>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QProcess>

#include "form.h"
#include "canprogworker.h"
#include "qtBlokLib/parser.h"

// Драйвер CAN
Can *can;
#ifdef LIB_CAN_NICK
#include "qtCanLib/cannick.h"
#elif defined(LIB_SOCKET_CAN)
#include "qtCanLib/socketcan.h"
#else
#include "qtCanLib/dummycan.h"
#endif

Parser *blokMessages;

using namespace IsoTpFrames;
using namespace IsoTp;
using namespace Fudp;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    #ifdef LIB_CAN_NICK
    can = new CanNick ();
    #elif defined(LIB_SOCKET_CAN)
    can = new SocketCan ();
    #else
    can = new DummyCan ();
    #endif

    blokMessages = new Parser (can);

    CanProgWorker canProgWorker (can, "C:/MonMSUL/prop.txt");
    Form w(&canProgWorker, blokMessages);
    w.show();
    return a.exec();
}
