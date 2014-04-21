#include "processmanager.h"

#include <QTimer>

ProcessManager::ProcessManager(QDir rootDir, bool running, QObject *parent) :
    QObject (parent),
    rootDir (rootDir),
    running (running),
    process (this)
{
    QObject::connect (&process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(onProcessStateChanged(QProcess::ProcessState)));

    if (running)
        start ();
}

void ProcessManager::keepRunning(bool run)
{
    if (running != run)
    {
        running = run;

        if (run)
            start ();
        else
            stop ();
    }
}

void ProcessManager::start()
{
    process.setWorkingDirectory (rootDir.path ());
    process.start ("start");
}

void ProcessManager::stop()
{
    QProcess stopScript;
    stopScript.setWorkingDirectory (rootDir.path ());
    stopScript.start ("stop");
    stopScript.waitForFinished (5000);
    stopScript.kill ();

    process.kill ();
}

void ProcessManager::onProcessStateChanged(QProcess::ProcessState state)
{
    if ( running && state == QProcess::NotRunning )
        QTimer::singleShot (100, this, SLOT(start()));

    if ( !running && state != QProcess::NotRunning )
        QTimer::singleShot (100, this, SLOT(stop()));
}
