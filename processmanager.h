#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QDir>
#include <QProcess>
#include <QTime>

class IProcess : public QObject
{
    Q_OBJECT
public:
    explicit IProcess (QObject *parent) : QObject (parent) { }

    virtual QProcess::ProcessState state () const = 0;

public slots:
    virtual void start () = 0;
    virtual void stop () = 0;

signals:
    void stateChanged(QProcess::ProcessState state);
};

class Process : public IProcess
{
    Q_OBJECT
public:
    explicit Process (QString programName, QObject *parent = 0)
        : IProcess (parent), process (this), name (programName)
    {
        QObject::connect(&process, SIGNAL(stateChanged(QProcess::ProcessState)),
                         this, SLOT(passStateChangedSignal(QProcess::ProcessState)));
    }

    virtual QProcess::ProcessState state () const { return process.state(); }
    QProcess &qProcess () { return process; }

public slots:
    void start () { process.start(name); }
    void start (const QStringList &arguments) { process.start(name, arguments); }
    void stop () { process.kill(); }

private slots:
    void passStateChangedSignal (QProcess::ProcessState state) { emit stateChanged(state); }

private:
    QProcess process;
    QString name;
};

class SustainedProcess : public QObject
{
    Q_OBJECT
public:
    explicit SustainedProcess (IProcess *process, QObject *parent = 0);

    enum SustainMode {
        Uncontrolled = 0,
        KeepRunning = 1,
        KeepStopped = 2
    };
    void setSustainMode (SustainMode mode);
    SustainMode sustainMode () const;

private slots:
    void onProcessStateChanged (QProcess::ProcessState sustain);

private:
    IProcess *process;
    SustainMode sustain;
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
    Process startScript, stopScript, deployScript;
    SustainedProcess process;
};

#endif // PROCESSMANAGER_H
