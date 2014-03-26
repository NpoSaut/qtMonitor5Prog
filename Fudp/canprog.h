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

class CanProg : public QObject
{
    Q_OBJECT
public:
    explicit CanProg(Can *can, PropStore *hwStore, PropStore *pStore, QDir rootDir, QObject *parent = 0);

signals:
    // Наверх
    void sendState(QString state);
    void sendFileInfo(QString fileName, qint32 fileSize);
    void progModeChanged (bool progMode);
    void crcCheckChanged (bool crcOk);
    void noSerialNumber();

    // private
    void sendAnswerToBroadcast(DeviceTicket myTicket);
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

private slots:
    void connect(const DeviceTicket &tickets);
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
    void exitProgMode ();
    bool checkFirmware ();
    bool saveChanges();
    void discardChanges ();
    void takeFileList();
    void sessionTimeOut ();

private:
    static const int FuInit = 0x66a8;
    static const int FuProg = 0x66c8;
    static const int FuDev =  0x66e8;

    WorkingWithFudpMessage worker;
    PropStore *loaderPropStore;
    PropStore *hwStore;
    PropStore *pStore;
    QMap<QString, DevFileInfo> fileList;
    DeviceTicket myTicket;
    QDir rootDir;
    bool progMode;
    bool isSerialNumberSet;

    QStringList parseDir(const QDir dir);
    bool isLoaderProperty (qint8 key);
    bool isHwProperty (qint8 key);
};
}
#endif // CANPROG_H
