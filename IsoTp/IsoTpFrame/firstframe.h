#ifndef FIRSTFRAME_H
#define FIRSTFRAME_H

#include "isotpframe.h"

namespace IsoTpFrames
{
  class FirstFrame: public IsoTpFrame
  {
  public:
    FirstFrame(const std::vector<byte> &data, int packetSize);
    CanFrame getCanFrame();
    const std::vector<byte> &getData() const;
    int getPacketSize() const;


  private:
    const int type = IsoTpFrameType(firstFrame);
    int packetSize;
    std::vector<byte> data;

  };
}

#endif // FIRSTFRAME_H
