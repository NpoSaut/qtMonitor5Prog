#include "singleframe.h"

namespace IsoTpFrames
{
  SingleFrame::SingleFrame(const std::vector<byte> &data)
    : data(data)
  {
  }

  const std::vector<byte> &SingleFrame::getData() const
  {
    return data;
  }

  CanFrame SingleFrame::getCanFrame()
  {
    CanFrame canFrame;
    std::vector<byte> v = data;
    v.insert(v.begin(), ((byte)((byte)type & 0x0f) << 4  | data.size() & 0x0f));
    canFrame.setData(v);
    return canFrame;
  }
}
