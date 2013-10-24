#include "logwriter.h"

LogWriter::LogWriter(QObject *parent) :
    Singletone<LogWriter>(*this),
    QObject(parent)
{
}

//void LogWriter::write(const std::vector<unsigned char> &data)
//{
//    QDateTime currentTime(QDateTime::currentDateTime());
//    QString text;
//    text.append(currentTime.toString("hh:mm:ss.zzz") + " ");
//    text.append(vectorToQString(data));
//    emit getText(text);
//}

void LogWriter::write(const QString &data)
{
    QString text;
    text.append(QDateTime::currentDateTime().toString("hh:mm:SS.zzz") + " ");
    text.append(" " + data);
    emit getText(text);
}

void LogWriter::write(const CanInternals::StructForDrv &data)
{
    QString text;
    text.append(QDateTime::currentDateTime().toString("hh:mm:SS.zzz") + " ");
    text.append(QString::number((data.id >> 18) * 0x20 + 0x8, 16) + "|");
    for(int i = 0; i<sizeof(data.data); i++)
            text.append(QString::number(data.data[i], 16));
    emit getText(text);
}

void LogWriter::write(const CanInternals::TransmitData &data)
{
    QString text;
    text.append(QDateTime::currentDateTime().toString("hh:mm:SS.zzz") + " ");
    text.append(QString::number((data.id >> 18) * 0x20 + 0x8, 16));
    for(int i = 0; i<sizeof(data.data); i++)
            text.append(QString::number(data.data[i], 16));
    emit getText(text);
}

QString LogWriter::vectorToQString(std::vector<unsigned char> data)
{
    QString str;
    for(int i = 0; i<data.size(); i++)
        str.append(QString::number(data.at(i)) + " ");
    return str;
}
