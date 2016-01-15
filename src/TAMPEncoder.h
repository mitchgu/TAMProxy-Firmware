#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include <Encoder.h>

namespace tamproxy {

class Encoder : public Device {
private:
    uint8_t _pinA;
    uint8_t _pinB;

    ::Encoder _enc;
    volatile uint32_t _count;

public:
    Encoder(uint8_t pinA, uint8_t pinB);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}
#endif