#include "progfirmcorrupt.h"

using namespace FudpMessage;

ProgFirmCorrupt::ProgFirmCorrupt()
{
}

std::vector<byte> ProgFirmCorrupt::encode()
{
    std::vector<byte> mes (1);
    mes[0] = 0xFF;
    return mes;
}

void ProgFirmCorrupt::decode(const std::vector<byte> &data)
{
}
