#include "flowcontrolframe.h"

namespace IsoTpFrames
{
  FlowControlFrame::FlowControlFrame(FlowControlFlag flag, byte blockSize, int separationTime)
    : flag(flag), blockSize(blockSize), separationTime(separationTime)
  {
  }
  FlowControlFrame::FlowControlFrame(FlowControlFlag flag, byte blockSize, byte code)
    : flag(flag), blockSize(blockSize), separationTime(setSeparationTimeFromCode(code))
  {
  }

  byte FlowControlFrame::getBlockSize()
  {
    return (byte)blockSize;
  }

  byte FlowControlFrame::getSeparationTime()
  {
    return (byte)separationTime;
  }

  int FlowControlFrame::setSeparationTimeFromCode(byte code)
  {
    return code < 0x7f ?
          code : 1;
  }

  FlowControlFlag FlowControlFrame::getFrlag()
  {
    return flag;
  }

  void FlowControlFrame::setFlag(FlowControlFlag flag)
  {
    this->flag = flag;
  }

  CanFrame FlowControlFrame::getCanFrame()
  {
    CanFrame canFrame;
    std::vector<byte> v;
    v.insert(v.end(), ((byte)((byte)type & 0x0f) << 4  | (byte)flag & 0x0f));
    v.insert(v.end(), (byte)blockSize);
    v.insert(v.end(), (byte)separationTime);
    canFrame.setData(v);
    return canFrame;
  }
}
