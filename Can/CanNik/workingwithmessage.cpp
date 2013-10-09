#include "workingwithmessage.h"


namespace CanInternals
{
  CanDriver canDrv;

  void transmit(CanFrame frame)
  {      
    TransmitData td;
    td.control = 1;
    td.counter = 1;
    td.id = frame.getId() << 18 ;
    td.period = 50;
    td.dataLength = 8;
    for(int i = 0; i < 8; i++)
      td.data[i] = frame[i+1];
    canDrv.transmitMessage(td);
  }

  bool receive(CanFrame &receivedMessage)
  {
    StructForDrv buff;
    bool isMessage = canDrv.receiveMessage(buff);
    if(isMessage)
      {
        receivedMessage.setId(buff.id >> 18);
        std::vector<byte> v(buff.data, buff.data + buff.datalen);
        receivedMessage.setData(v);
      }
    return isMessage;

  }

  ReceiveMessageThread receiveMessagetLoop ("can0");

  ReceiveMessageThread::ReceiveMessageThread(QString interfaceName)
  {
    this->start();
  }

  void ReceiveMessageThread::run()
  {
    while(true)
      {
        CanFrame receivedMessage;
        if (receive(receivedMessage))
          {
            emit messageReceived(receivedMessage);
          }
      }
  }
}


