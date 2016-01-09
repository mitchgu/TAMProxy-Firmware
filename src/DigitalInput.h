#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <cstdint>
#include <vector>
#include "Device.h"

namespace tamproxy {

// A class that just reads a digital input pin
class DigitalInput : public Device {
private:
    uint8_t _pin;
public:
    DigitalInput(uint8_t pin, uint8_t pullup);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}

#endif