#ifndef SINGLEFRAME_H
#define SINGLEFRAME_H

#include "isotpframe.h"
namespace IsoTpFrames
{
  class SingleFrame : public IsoTpFrame
  {
  public:
    SingleFrame(const std::vector<byte> &data);
    CanFrame getCanFrame();
    const std::vector<byte> &getData() const;

  private:
    const int type = IsoTpFrameType(singleFrame);
    std::vector<byte> data;
  };
}

#endif // SINGLEFRAME_H
