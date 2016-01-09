#ifndef DIGITAL_OUTPUT_H
#define DIGITAL_OUTPUT_H

#include <cstdint>
#include <vector>
#include "Device.h"

namespace tamproxy {

// A class that just writes to a digital output pin
class DigitalOutput : public Device {
private:
    uint8_t _pin;
public:
    DigitalOutput(uint8_t pin);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}
#endif