#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <QObject>
#include <QDateTime>
#include <vector>
#include <QTextEdit>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include "singletone.h"
#include "qtCanLib/CanNick/candriver.h"

#define LOG_WRITER LogWriter::instance()

class LogWriter : public QObject, public Singletone<LogWriter>
{
    Q_OBJECT
public:
    explicit LogWriter(QObject *parent = 0);

    void write(const QString &data, QColor color, int error = 0);
    void write(const CanInternals::StructForDrv &data);
    void write(const CanInternals::TransmitData &data);

    void installLog();
    void finishLog();

private:
    const int FuInit = 0x66a8;
    const int FuProg = 0x66c8;
    const int FuDev =  0x66e8;
    QTextEdit *container;
    QFile logFile;
    QTextStream logStream;
    QStringList buffer;

    QString ToQString(CanInternals::StructForDrv data);
    QString ToQString(CanInternals::TransmitData data);

signals:
    void setText(QString data);
    void setColor(QColor color);

public slots:
    
};

#endif // LOGWRITER_H
