#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <vector>

// A packet struct. Pretty self explanatory.

namespace tamproxy {

struct Packet {
public:
    uint16_t id;
    uint8_t dest;
    std::vector<uint8_t> payload;
};

}
#endif