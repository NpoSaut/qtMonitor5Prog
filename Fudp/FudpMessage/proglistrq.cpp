#include "proglistrq.h"

ProgListRq::ProgListRq()
{
}

std::vector<byte> ProgListRq::encode()
{
    std::vector<byte> buffer;
    buffer.insert(buffer.end(), MessageId(progListRq));
    return buffer;
}

void ProgListRq::decode(std::vector<byte> &data)
{

}
