#include "progpong.h"

#include <QMetaType>

using namespace FudpMessage;

ProgPong::ProgPong(quint8 counter, ProgPong::Status status)
    : counter (counter), status (status)
{
    qRegisterMetaType<ProgPong::Status>("ProgPongStatus");
}

std::vector<byte> ProgPong::encode()
{
    std::vector<byte> out (3, 0);
    out[0] = progPong;
    out[1] = counter;
    out[2] = status;
    return out;
}

void ProgPong::decode(const std::vector<byte> &data)
{
    counter = data[1];
    status = Status(data[2]);
}
