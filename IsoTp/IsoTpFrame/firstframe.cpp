#include "firstframe.h"

namespace IsoTpFrames
{
  FirstFrame::FirstFrame(const std::vector<byte> &data, int packetSize)
    : data(data), packetSize(packetSize)
  {
  }

  const std::vector<byte> &FirstFrame::getData() const
  {
    return data;
  }

  int FirstFrame::getPacketSize() const
  {
    return packetSize;
  }

  CanFrame FirstFrame::getCanFrame()
  {
    CanFrame canFrame;
    std::vector<byte> v = data;
    v.insert(v.begin(),packetSize & 0x0ff);
    v.insert(v.begin(),((byte)((byte)type & 0x0f) << 4  | (packetSize & 0xf00) >> 8));
    canFrame.setData(v);
    return canFrame;
  }
}
