#ifndef FLOWCONTROLFRAME_H
#define FLOWCONTROLFRAME_H

#include "isotpframe.h"
namespace IsoTpFrames
{
  enum FlowControlFlag
  {
    ClearToSend = 0x00,
    Wait = 0x01,
    Abort = 0x02
  };

  class FlowControlFrame : public IsoTpFrame
  {
  public:
    FlowControlFrame(FlowControlFlag flag, byte blockSize, int separationTime);
    FlowControlFrame(FlowControlFlag flag, byte blockSize, byte separationTime);
    CanFrame getCanFrame();
    byte getBlockSize();

    byte getSeparationTime();
    int setSeparationTimeFromCode(byte code);

    FlowControlFlag getFrlag();
    void setFlag(FlowControlFlag flag);

  private:
    static const int type = IsoTpFrameType(flowControlFrame);
    FlowControlFlag flag;
    byte blockSize;
    int separationTime;
  };
}

#endif // FLOWCONTROLFRAME_H
