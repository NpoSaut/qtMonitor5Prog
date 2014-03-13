#ifndef ISOTPFRAME_H
#define ISOTPFRAME_H

#include "QtGlobal"
#include "qtCanLib/canframe.h"

typedef quint8 byte;

namespace IsoTpFrames
{
  enum IsoTpFrameType
  {
    singleFrame = 0x0,
    firstFrame = 0x1,
    consecutiveFrame = 0x2,
    flowControlFrame = 0x3
  };

  class IsoTpFrame
  {
  public:
    virtual CanFrame getCanFrame() = 0;
  };
}

#endif // ISOTPFRAME_H
