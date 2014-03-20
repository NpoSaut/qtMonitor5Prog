#include <QCoreApplication>
#include <QApplication>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QProcess>

#include "form.h"
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

using namespace IsoTpFrames;
using namespace IsoTp;
using namespace Fudp;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #ifdef LIB_CAN_NICK
    can = new CanNick ();
    #elif defined(LIB_SOCKET_CAN)
    can = new SocketCan ();
    #else
    can = new DummyCan ();
    #endif

        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    if (a.arguments ().size () >= 2)
    {
        CanProgWorker canProgWorker (can, a.arguments ().at(1), a.arguments ().at (2));
        Form w(&canProgWorker);
        w.show();
        return a.exec();
    }
    else
    {
        qDebug() << "Two arguments needed: ";
        qDebug() << "first - root directory";
        qDebug() << "second - path to prop.txt";
        qDebug() << "This is all.";
        return 1;
    }

}
