#ifndef ANALOG_OUTPUT_H
#define ANALOG_OUTPUT_H

#include <cstdint>
#include <vector>
#include "Device.h"

namespace tamproxy {

// A class that just writes to a digital output pin
class AnalogOutput : public Device {
private:
    uint8_t _pin;
public:
    AnalogOutput(uint8_t pin);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}

#endif