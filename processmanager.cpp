#include "processmanager.h"

ProcessManager::ProcessManager(QDir rootDir, bool running, QObject *parent) :
    QObject(parent),
    rootDir (rootDir),
    running (running),
    process()
{
    if (running)
        start ();
}

void ProcessManager::keepRunning(bool run)
{
    if (running != run)
    {
        if (run)
            start ();
        else
            stop ();

        running = run;
    }
}

void ProcessManager::start()
{
    QDir::setCurrent (rootDir.path ());
    QObject::connect (&process, SIGNAL(finished(int)), this, SLOT(onProcessTerminated(int)));
    process.start ("start");
}

void ProcessManager::stop()
{
    QObject::disconnect (this, SLOT(onProcessTerminated(int)));
    QDir::setCurrent (rootDir.path ());
    QProcess stopScript;
    stopScript.start ("stop");
    stopScript.waitForFinished (5000);
    stopScript.terminate ();
    process.terminate ();
}

void ProcessManager::onProcessTerminated(int status)
{
    if (running)
        start ();
    else
        stop ();
}
