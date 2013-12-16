#ifndef CANPROG_H
#define CANPROG_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QProcess>
#include "FudpMessage/workingwithfudpmessage.h"
#include "PropStore/propstore.h"
#include "Log/logwriter.h"
using namespace FudpMessage;

namespace Fudp
{

//void hideWindow();
//void showWindow();

class CanProg : public QObject
{
    Q_OBJECT
public:
    explicit CanProg(PropStore *pStore, QObject *parent = 0);
    QStringList parseDir(const QDir dir);

private:
    const int FuInit = 0x66a8;
    const int FuProg = 0x66c8;
    const int FuDev =  0x66e8;
    LogWriter log;
    WorkingWithFudpMessage worker;
    PropStore *pStore;
    QMap<QString, DevFileInfo> fileList;
    DeviceTickets myTicket;
    QTimer initWaitTimer;
    QProcess monitor;
    bool progMode;
    bool isSerialNumber;

    void progModeExit (int errorCode = 0);
    bool checkProgram ();
    bool saveChanges();
    void takeFileList();

signals:
    void sendAnswerToBroadcast(DeviceTickets myTicket);
    void sendProgStatus(QVector< QPair<quint8, qint32> > dictionary);
    void sendFileList(QMap<QString, DevFileInfo> list);
    void sendFile(qint8 errorCode, QByteArray data);
    void sendDeleteFileAck(qint8 errorCode);
    void sendDeleteAllFilesAck();
    void sendCreateFileAck(qint8 errorCode);
    void sendWriteFileAck(qint8 errorCode);
    void sendSetParamAck(qint8 errorCode);
    void sendDeleteParamAck(qint8 errorCode);
    void sendFirmCorrupt();
    void sendSubmitAck(qint8 errorCode);

    void sendState(QString state);
    void sendFileInfo(QString fileName, qint32 fileSize);
    void senValueReceiveBytes(qint32 receiveBytes);

    void noSerialNumber();

public slots:
    void connect(const DeviceTickets &tickets);
    void getFileList();
    void readFile(const QString &fileName, qint32 offset, qint32 readSize);
    void deleteFile(const QString &fileName);
    void deleteAllFiles(qint32 securityKey);
    void createFile(const QString &fileName, qint32 fileSize);
    void writeFile(const QString &fileName, qint32 offset, const QByteArray &data);
    void setParam(qint8 key, qint32 value);
    void deleteParam(qint8 key);
    void submit(qint8 subimtKey);
    void inputBlockSerialNumber(qint32 blockSerialNumber);
    void timeOut();
    void drvStart();

private slots:
    void start(int exitCode);
    void periodicalCheck();
};
}
#endif // CANPROG_H
