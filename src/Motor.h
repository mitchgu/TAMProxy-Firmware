#ifndef MOTOR_H
#define MOTOR_H

#include <cstdint>
#include <vector>
#include "Device.h"

// A class that that controls motor controllers that take a dir and pwm inputs

namespace tamproxy {

class Motor : public Device {
private:
    uint8_t _dirPin;
    uint8_t _pwmPin;
public:
    Motor(uint8_t dirPin, uint8_t pwmPin);
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}
#endif