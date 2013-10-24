#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <QObject>
#include <QDateTime>
#include <vector>
#include <QTextEdit>
#include "singletone.h"
#include "Can/CanNik/candriver.h"

#define LOG_WRITER LogWriter::instance()

class LogWriter : public QObject, public Singletone<LogWriter>
{
    Q_OBJECT
public:
    explicit LogWriter(QObject *parent = 0);

//    void write(const std::vector<unsigned char> &data);
    void write(const QString &data);
    void write(const CanInternals::StructForDrv &data);
    void write(const CanInternals::TransmitData &data);

private:
    QTextEdit *container;

    QString vectorToQString(std::vector<unsigned char> data);

signals:
    void getText(QString data);

public slots:
    
};

#endif // LOGWRITER_H
