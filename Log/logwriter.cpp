#ifdef LIB_CAN_NICK // не должно это тут быть

#include "logwriter.h"
#include "QDir"

LogWriter::LogWriter(QObject *parent) :
    logFile(QString("C:/MonMSUL/log%1.log").arg(QDateTime::currentDateTime().toString("hhmm"))),
    buffer(),
    logStream(&logFile),
    Singletone<LogWriter>(*this),
    QObject(parent)
{
    QDir dir("C:/MonMSUL");

    QStringList loggers = dir.entryList(QStringList("*.log"), QDir::Files, QDir::Time);
    if(loggers.size() >= 7)
    {
        QFile("C:/MonMSUL/" + loggers.at(loggers.size()-1)).remove();
    }
}

void LogWriter::installLog()
{
    if(logFile.open(QFile::WriteOnly | QIODevice::Unbuffered | QIODevice::Append))
    {
        logStream << "|----------------------------------------------------------------|\r\n";
        logStream << QString(tr("Начало записи %1\r\n")).arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
        logStream << tr("Маркеры: '>>' входящее сообщение, '<<' исходящее сообщение\r\n\r\n");
    }

    buffer.clear();
}

void LogWriter::finishLog()
{
    logFile.close();
}

void LogWriter::write(const QString &data, QColor color, int error)
{
    QString text;
    text.append(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
    text.append(" " + data);

    if (error)
        for(int i = 0; i < buffer.length(); i++)
            logStream << buffer.at(i);

    logStream << text + "\r\n";

    emit setColor(color);
    emit setText(data);
}

void LogWriter::write(const CanInternals::StructForDrv &data)
{
    int descr = (data.id >> 18) * 0x20 + 0x8;
    if((descr == FuInit) || (descr == FuProg))
    {
        if(buffer.length() < 66)
            buffer.append(ToQString(data) + "\r\n");
        else
        {
            buffer.erase(buffer.begin());
            buffer.append(ToQString(data) + "\r\n");
        }
    }
}

void LogWriter::write(const CanInternals::TransmitData &data)
{

    if(buffer.length() < 66)
        buffer.append(ToQString(data) + "\r\n");
    else
    {
        buffer.erase(buffer.begin());
        buffer.append(ToQString(data) + "\r\n");
    }
}

QString LogWriter::ToQString(CanInternals::StructForDrv data)
{
    QString time;
    time.append(QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + " >> ");

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
    time.append(QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + " << ");

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

#endif // LIB_CAN_NICK
