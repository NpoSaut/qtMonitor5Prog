#include <QCoreApplication>
#include <QApplication>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QProcess>
#include <tchar.h>
#include "form.h"
#include "canprogworker.h"

using namespace CanInternals;
using namespace IsoTpFrames;
using namespace IsoTp;
using namespace Fudp;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    CanProgWorker canProgWorker ("C:/MonMSUL/prop.txt");
    Form w(&canProgWorker);
    w.show();
    return a.exec();
}
