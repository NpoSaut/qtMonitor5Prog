#ifndef CANFRAME_H
#define CANFRAME_H

#include <QDebug>
#include <windows.h>
#include <vector>

namespace CanInternals
{
  typedef unsigned char byte;

  class CanFrame
  {
  public:
    CanFrame();
    CanFrame(int id, int size, const std::vector<byte> &data);
    CanFrame(int descriptor, const std::vector<byte> &data);

    int getId() const;
    void setId(const int &id);

    int getDescriptor() const;
    void setDescriptor(int descriptor);

    const std::vector<byte> &getData() const;
    void setData(const std::vector<byte> &data);

    // !WARNING! --< CAN message byte numbers start from 1 >-- !WARNING!
    unsigned char &operator[] (int index);
    const unsigned char &operator[] (int index) const;

  private:
    std::vector<byte> data;
    int id;

    inline int checkSize(int size);
  };
}

#endif // CANFRAME_H
