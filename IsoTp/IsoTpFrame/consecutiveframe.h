#ifndef CONSECUTIVEFRAME_H
#define CONSECUTIVEFRAME_H

#include "isotpframe.h"

namespace IsoTpFrames
{
  class ConsecutiveFrame : public IsoTpFrame
  {
  public:
    ConsecutiveFrame(const std::vector<byte> &data, int index);
    CanFrame getCanFrame();
    int getIndex();
    const std::vector<byte> &getData() const;


  private:
    int index;
    const int type = IsoTpFrameType(consecutiveFrame);
    std::vector<byte> data;

  };
}

#endif // CONSECUTIVEFRAME_H
