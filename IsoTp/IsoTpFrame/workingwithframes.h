#ifndef WORKINGWITHFRAMES_H
#define WORKINGWITHFRAMES_H

#include <QObject>
#include "qtCanLib/can.h"
#include "singleframe.h"
#include "firstframe.h"
#include "consecutiveframe.h"
#include "flowcontrolframe.h"

namespace IsoTpFrames
{

  class WorkingWithFrames : public QObject
  {
    Q_OBJECT
  public:
    explicit WorkingWithFrames(Can *can, QObject *parent = 0);

    void setTransmitDescriptor(int td);
    int getTransmitDescriptor();

    void setAcknowlegmentDescriptor(int ad);
    int getAcknowlegmentDescriptor();

  private:
//    CanFrame convertToCanFrame(SingleFrame &frame);
//    CanFrame convertToCanFrame(FirstFrame &frame);
//    CanFrame convertToCanFrame(ConsecutiveFrame &frame);
//    CanFrame convertToCanFrame(FlowControlFrame &frame);
    const Can *can;
    int transmitDescriptor;
    int acknowlegmentDescriptor;

  signals:
    void transmitCanFrame(CanFrame frame);

    void receiveSingleFrame(SingleFrame frame);
    void receiveFirstFrame(FirstFrame frame);
    void receiveConsecutiveFrame(ConsecutiveFrame frame);
    void receiveFlowControlFrame(FlowControlFrame frame);

  public slots:
    void transmitSingleFrame(SingleFrame frame);
    void transmitFirstFrame(FirstFrame frame);
    void transmitConsecutiveFrame(ConsecutiveFrame frame);
    void transmitFlowControlFrame(FlowControlFrame frame);

    void receiveCanFrame(const CanFrame &frame);
  };
}
#endif // WORKINGWITHFRAMES_H
