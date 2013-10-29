#include "logwriter.h"

LogWriter::LogWriter(QObject *parent) :
    logFile("log.log"),
    logStream(&logFile),
    Singletone<LogWriter>(*this),
    QObject(parent)
{

}

void LogWriter::installLog()
{
    logFile.open(QFile::WriteOnly | QIODevice::Unbuffered | QIODevice::Append);
    logStream << QString(tr("Начало записи в %1\r\n")).arg(QDateTime::currentDateTime().toString("dd:mm:yyyy hh:mm"));
    logStream << tr("Маркеры: '>>' входящее сообщение, '<<' исходящее сообщение\r\n\r\n");
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
    logStream << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
    logStream << ToQString(data) + "\r\n";

    emit setColor(QColor(255, 255, 0));
    emit setText(ToQString(data));
}

void LogWriter::write(const CanInternals::TransmitData &data)
{
    logStream << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
    logStream << ToQString(data) + "\r\n";

    emit setColor(QColor(0, 255, 255));
    emit setText(ToQString(data));
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
