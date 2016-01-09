#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H

#include <cstdint>
#include <vector>
#include "Device.h"

namespace tamproxy {

// A class that just reads a digital input pin
class AnalogInput : public Device {
private:
    uint8_t _pin;
public:
    AnalogInput(uint8_t pin);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}
#endif
