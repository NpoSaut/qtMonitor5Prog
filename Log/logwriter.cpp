#include "logwriter.h"
#include "QDir"

LogWriter::LogWriter(QObject *parent) :
    logFile(QString("C:\\MonMSUL\\%1.log").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy"))),
    logStream(&logFile),
    Singletone<LogWriter>(*this),
    QObject(parent)
{

    QDir dir = QDir::current();

    QStringList loggers = dir.entryList(QStringList("*.log"),QDir::Files, QDir::Name);
    if(loggers.size() > 7)
    {
        QFile(loggers.at(0)).remove();
    }
}

void LogWriter::installLog()
{
    if(logFile.open(QFile::WriteOnly | QIODevice::Unbuffered | QIODevice::Append))
    {
        logStream << QString(tr("Начало записи в %1\r\n")).arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
        logStream << tr("Маркеры: '>>' входящее сообщение, '<<' исходящее сообщение\r\n\r\n");
    }
}

void LogWriter::finishLog()
{
    logFile.close();
}

void LogWriter::write(const QString &data, QColor color)
{
    QString text;
    text.append(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
    text.append(" " + data);

    logStream << text + "\r\n";

    emit setColor(color);
    emit setText(text);
}

void LogWriter::write(const CanInternals::StructForDrv &data)
{
    int descr = (data.id >> 18) * 0x20 + 0x8;
    if((descr == FuInit) || (descr == FuProg))
    {
        logStream << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
        logStream << ToQString(data) + "\r\n";

//        emit setColor(QColor(255, 255, 0));
//        emit setText(ToQString(data));
    }
}

void LogWriter::write(const CanInternals::TransmitData &data)
{
    logStream << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
    logStream << ToQString(data) + "\r\n";

//    emit setColor(QColor(0, 255, 255));
//    emit setText(ToQString(data));
}

QString LogWriter::ToQString(CanInternals::StructForDrv data)
{
    QString time;
    time.append(/*QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + */">> ");

    QString mess;
    mess.sprintf(" %04x | ", (data.id >> 18) * 0x20 + 0x8);

    for(int i = 0; i<data.datalen; i++)
    {
        QString byte;
        byte.sprintf("%02x ", data.data[i]);
        mess.append(byte);
    }

    return time + mess;
}

QString LogWriter::ToQString(CanInternals::TransmitData data)
{
    QString time;
    time.append(/*QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + */"<< ");

    QString mess;
    mess.sprintf(" %04x | ", (data.id >> 18) * 0x20 + 0x8);

    for(int i = 0; i<data.dataLength; i++)
    {
        QString byte;
        byte.sprintf("%02x ", data.data[i]);
        mess.append(byte);
    }

    return time + mess;
}
