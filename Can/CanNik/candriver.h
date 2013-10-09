#ifndef CANDRIVER_H
#define CANDRIVER_H

#include <QObject>
#include <QDebug>
#include <windows.h>
#include <vector>
#include "Can/canframe.h"

namespace CanInternals
{
  struct StructForDrv
  {
    DWORD id;
    byte datalen;
    byte data[8];
  };

  struct TransmitData
  {
    DWORD id;
    DWORD period;
    int counter;
    byte control;
    byte dataLength;
    byte data[8];
  };

  class CanDriver : public QObject
  {
    Q_OBJECT
  public:
    explicit CanDriver(QObject *parent = 0);
    ~CanDriver();
    bool receiveMessage(StructForDrv &buff);
    int transmitMessage(TransmitData &td);

    bool canEnable;

  private:
    bool deviceIo(DWORD code, LPVOID *inBuffer, DWORD inBufferSize, LPVOID *outBuffer, DWORD outBufferSize);
    void transmitLog(const TransmitData &td);
    void readLog(StructForDrv &cm);
    int getNumberOfMessageInQueue();
    void init();

    const LPCWSTR name = L"\\\\.\\DriverCANDevice0";
    const HANDLE driverHandle = INVALID_HANDLE_VALUE;
    const DWORD ioctlCanInit = CTL_CODE(0x80FF, 9, METHOD_BUFFERED, FILE_ANY_ACCESS);
    const DWORD ioctlCanTransmit = CTL_CODE(0x80FF, 10, METHOD_BUFFERED, FILE_ANY_ACCESS);
    const DWORD ioctlCanRead = CTL_CODE(0x80FF, 12, METHOD_BUFFERED, FILE_ANY_ACCESS);
    const DWORD ioctlCanInQueue = CTL_CODE(0x80FF, 13, METHOD_BUFFERED, FILE_ANY_ACCESS);
    const DWORD ioctlCanClear = CTL_CODE(0x80FF, 14, METHOD_BUFFERED, FILE_ANY_ACCESS);
    const HANDLE transmitMutex = CreateMutex(NULL, false, NULL);
    const HANDLE receiveMutex = CreateMutex(NULL, false, NULL);


  signals:


  public slots:

  };
}
#endif // CANDRIVER_H
