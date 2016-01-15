#ifndef SERVO_H
#define SERVO_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include <Servo.h>

// A class for controlling servos

namespace tamproxy {

class Servo : public Device {
private:
    uint8_t _pwmPin;
	::Servo _servo;
	
public:
    Servo(uint8_t pwmPin);
    ~Servo();
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

}
#endif
