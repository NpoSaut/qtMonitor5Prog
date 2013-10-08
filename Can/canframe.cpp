#include "canframe.h"

namespace CanInternals
{
  CanFrame::CanFrame()
    : data(), id(0)
  {
  }

  CanFrame::CanFrame(int id, int size, const std::vector<byte> &data)
    : data (data), id (id)
  {
    this->data.resize(checkSize(size));
  }

  CanFrame::CanFrame(int descriptor, const std::vector<byte> &data)
    : data (data)
  {
    setDescriptor(descriptor);
  }

  int CanFrame::getId() const
  {
    return id;
  }

  void CanFrame::setId(const int &id)
  {
    this->id = id;
  }

  int CanFrame::getDescriptor() const
  {
    return id * 0x20 + data.size();
  }

  void CanFrame::setDescriptor(int descriptor)
  {
    id = descriptor / 0x20;
    int size = checkSize(descriptor & 0xf);
    data.resize(size);
  }

  const std::vector<byte> &CanFrame::getData() const
  {
    return data;
  }

  void CanFrame::setData(const std::vector<byte> &data)
  {
    this->data = data;
  }

  unsigned char &CanFrame::operator [](int index)
  {
      return data[index-1];
  }

  const unsigned char &CanFrame::operator [](int index) const
  {
      return data[index-1];
  }

  int CanFrame::checkSize(int size)
  {
    if (size < 0) return 0;
    if (size > 8) return 8;
    return size;
  }
}
