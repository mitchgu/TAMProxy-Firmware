#ifndef DIGITAL_OOUTPUT_H
#define DIGITAL_OOUTPUT_H

#include <cstdint>
#include <vector>
#include "Device.h"

// A class that just writes to a digital output pin

class DigitalOutput : public Device {
private:
    uint8_t _pin;
public:
    DigitalOutput(uint8_t pin);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

#endif