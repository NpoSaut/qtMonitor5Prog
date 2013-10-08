#ifndef CAN_H
#define CAN_H

#include <QObject>
#include "canframe.h"

namespace CanInternals
{
  class Can : public QObject
  {
    Q_OBJECT
  public:
    explicit Can(QObject *parent = 0);

  signals:
    void messageReceived(CanFrame frame);


  public slots:
    void getMessageFromDriver(CanFrame frame);
    void transmitMessage(CanFrame frame);
  };
  extern Can canDev;
}
#endif // CAN_H
