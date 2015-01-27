#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QDir>
#include <QProcess>
#include <QTime>

class NamedProcess : public QProcess
{
    Q_OBJECT
public:
    explicit NamedProcess (QString programName, QObject *parent = 0)
        : QProcess (parent), name (programName)
    { }

public slots:
    void start () { QProcess::start(name); }
    void start (const QStringList &arguments) { QProcess::start(name, arguments); }

private:
    QString name;
};

class SustainedProcess : public NamedProcess
{
    Q_OBJECT
public:
    explicit SustainedProcess (QString programName, QObject *parent = 0);

    enum SustainMode {
        Uncontrolled = 0,
        KeepRunning = 1,
        KeepStopped = 2
    };
    void setSustainMode (SustainMode mode);
    SustainMode sustainMode () const;

private slots:
    void onProcessStateChanged (QProcess::ProcessState sustain);
    void teststart();
    void testkill();

private:
    SustainMode sustain;
    QString name;
    QTime time;
};

class ProcessManager : public QObject
{
    Q_OBJECT
public:
    explicit ProcessManager(QDir binDir, QDir datDir, QDir tmpDir, QObject *parent = 0);
    
public slots:
    void start ();
    void stop ();
    void deploy (int versionFrom, int subversionFrom);

private:
    const QDir binDir, datDir, tmpDir;
    SustainedProcess startScript;
    NamedProcess stopScript, deployScript;
};

#endif // PROCESSMANAGER_H
