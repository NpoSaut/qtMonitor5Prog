#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QDir>
#include <QProcess>

class ProcessManager : public QObject
{
    Q_OBJECT
public:
    explicit ProcessManager(QDir rootDir = QDir::currentPath (), bool running = false, QObject *parent = 0);
    
public slots:
    void keepRunning (bool run);

protected slots:
    virtual void onProcessTerminated (int status);

protected:
    virtual void start ();
    virtual void stop ();

private:
    const QDir rootDir;
    QProcess process;
    bool running;
};

#endif // PROCESSMANAGER_H
