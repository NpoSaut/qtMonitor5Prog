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
#include "Can/CanNik/candriver.h"

#define LOG_WRITER LogWriter::instance()

class LogWriter : public QObject, public Singletone<LogWriter>
{
    Q_OBJECT
public:
    explicit LogWriter(QObject *parent = 0);

    void write(const QString &data, QColor color);
    void write(const CanInternals::StructForDrv &data);
    void write(const CanInternals::TransmitData &data);

    void installLog();
    void finishLog();

private:
    const int FuInit = 0xfc08;
    const int FuProg = 0xfc28;
    const int FuDev =  0xfc48;
    QTextEdit *container;
    QFile logFile;
    QTextStream logStream;

    QString ToQString(CanInternals::StructForDrv data);
    QString ToQString(CanInternals::TransmitData data);

signals:
    void setText(QString data);
    void setColor(QColor color);

public slots:
    
};

#endif // LOGWRITER_H
