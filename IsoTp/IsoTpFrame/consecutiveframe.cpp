#include "consecutiveframe.h"

namespace IsoTpFrames
{
  ConsecutiveFrame::ConsecutiveFrame(const std::vector<byte> &data, int index)
    :data(data), index(index)
  {
  }

  const std::vector<byte> &ConsecutiveFrame::getData() const
  {
    return data;
  }

  CanFrame ConsecutiveFrame::getCanFrame()
  {
    CanFrame canFrame;
    std::vector<byte> v = data;
    v.insert(v.begin(),((byte)((byte)type & 0x0f) << 4  | index & 0x0f));
    canFrame.setData(v);
    return canFrame;
  }

  int ConsecutiveFrame::getIndex()
  {
    return index;
  }
}
