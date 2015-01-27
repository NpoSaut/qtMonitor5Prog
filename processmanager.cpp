#include "processmanager.h"

#include <QTimer>
#include <QDebug>

SustainedProcess::SustainedProcess(QString programName, QObject *parent)
    : NamedProcess (programName, parent)
{
     QObject::connect (this, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(onProcessStateChanged(QProcess::ProcessState)));
     time.restart();
}

void SustainedProcess::setSustainMode(SustainedProcess::SustainMode mode)
{
    if (sustain != mode)
    {
        sustain = mode;
        onProcessStateChanged(state());
    }
}

SustainedProcess::SustainMode SustainedProcess::sustainMode() const
{
    return sustain;
}

void SustainedProcess::onProcessStateChanged(QProcess::ProcessState state)
{
    if ( sustain == KeepRunning && state == QProcess::NotRunning )
        QTimer::singleShot (1000, this, SLOT(teststart()));

    if ( sustain == KeepStopped && state != QProcess::NotRunning )
        QTimer::singleShot (1000, this, SLOT(testkill()));
}

void SustainedProcess::teststart()
{
    qDebug() << "start" << time.elapsed();
    start();
}

void SustainedProcess::testkill()
{
    qDebug() << "kill" << time.elapsed();
    kill();
}


ProcessManager::ProcessManager(QDir binDir, QDir datDir, QDir tmpDir, QObject *parent) :
    QObject (parent),
    binDir (binDir), datDir (datDir), tmpDir (tmpDir),
    startScript ("start", this), stopScript ("stop", this), deployScript ("deploy", this)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("BIN", binDir.absolutePath());
    env.insert("DAT", datDir.absolutePath());
    env.insert("TMP", tmpDir.absolutePath());
//    env.insert("PATH", env.value("PATH") + ":" + binDir.path());
    startScript.setProcessEnvironment(env);

    QProcess *scripts[3] = {&startScript, &stopScript, &deployScript};
    for (int i = 0; i < 3; i ++)
    {
        scripts[i]->setWorkingDirectory(binDir.path ());
        scripts[i]->setProcessEnvironment(env);
    }
}

void ProcessManager::start()
{
    startScript.setSustainMode(SustainedProcess::KeepRunning);
}

void ProcessManager::stop()
{
    startScript.setSustainMode(SustainedProcess::Uncontrolled);

    stopScript.start();
    if ( !stopScript.waitForFinished (5000) )
        stopScript.kill ();
    qDebug() << "stopScript killed";

    startScript.setSustainMode(SustainedProcess::KeepStopped);
}

void ProcessManager::deploy(int versionFrom, int subversionFrom)
{
    QStringList arguments;
    arguments << QString::number(versionFrom) << QString::number(subversionFrom);

    deployScript.start(arguments);
    if ( !deployScript.waitForFinished(30000) )
        deployScript.kill();
}
