#include "processmanager.h"

#include <QTimer>

// --------------------------- SustainedProcess ------------------------------------

SustainedProcess::SustainedProcess(IProcess *process, QObject *parent)
    : process (process), QObject (parent)
{
     QObject::connect (process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(onProcessStateChanged(QProcess::ProcessState)));
}

void SustainedProcess::setSustainMode(SustainedProcess::SustainMode mode)
{
    if (sustain != mode)
    {
        sustain = mode;
        onProcessStateChanged(process->state());
    }
}

SustainedProcess::SustainMode SustainedProcess::sustainMode() const
{
    return sustain;
}

void SustainedProcess::onProcessStateChanged(QProcess::ProcessState state)
{
    if ( sustain == KeepRunning && state == QProcess::NotRunning )
        QTimer::singleShot (1000, process, SLOT(start()));

    if ( sustain == KeepStopped && state != QProcess::NotRunning )
        QTimer::singleShot (1000, process, SLOT(kill()));
}

// ---------------------------- ProcessManager -------------------------------------

ProcessManager::ProcessManager(QDir binDir, QDir datDir, QDir tmpDir, QObject *parent) :
    QObject (parent),
    binDir (binDir), datDir (datDir), tmpDir (tmpDir),
    startScript ("start", this), stopScript ("stop", this), deployScript ("deploy", this),
    process (&startScript, this)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("BIN", binDir.absolutePath());
    env.insert("DAT", datDir.absolutePath());
    env.insert("TMP", tmpDir.absolutePath());
//    env.insert("PATH", env.value("PATH") + ":" + binDir.path());

    QProcess *processes[3] = {&startScript.qProcess(), &stopScript.qProcess(), &deployScript.qProcess()};
    for (int i = 0; i < 3; i ++)
    {
        processes[i]->setWorkingDirectory(binDir.path ());
        processes[i]->setProcessEnvironment(env);
    }
}

void ProcessManager::start()
{
    process.setSustainMode(SustainedProcess::KeepRunning);
}

void ProcessManager::stop()
{
    process.setSustainMode(SustainedProcess::Uncontrolled);

    stopScript.start();
    if ( !stopScript.qProcess().waitForFinished (5000) )
        stopScript.stop();

    process.setSustainMode(SustainedProcess::KeepStopped);
}

void ProcessManager::deploy(int versionFrom, int subversionFrom)
{
    QStringList arguments;
    arguments << QString::number(versionFrom) << QString::number(subversionFrom);

    deployScript.start(arguments);
    if ( !deployScript.qProcess().waitForFinished(30000) )
        deployScript.stop();
}
