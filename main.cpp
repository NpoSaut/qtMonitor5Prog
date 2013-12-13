//#include <QCoreApplication>
#include <QApplication>
#include <QTimer>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QProcess>
#include <tchar.h>
#include "form.h"
#include "Fudp/canprog.h"
#include "PropStore/simplefilepropstore.h"

using namespace CanInternals;
using namespace IsoTpFrames;
using namespace IsoTp;
using namespace Fudp;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QFile pFile("C:/MonMSUL/prop.txt");
    SimpleFilePropStore pStore(pFile);

    Form w(&pStore);
    w.show();
//    CanProg cp(&pStore);

    return a.exec();
}
