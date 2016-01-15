#include "Servo.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"
#include <Servo.h>

namespace tamproxy {

// Constructor takes a pwm pin
Servo::Servo(uint8_t pwmPin) {
    _pwmPin = pwmPin;
    _servo.attach(_pwmPin);
}

Servo::~Servo() {
    _servo.detach();
}

std::vector<uint8_t> Servo::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == SERVO_WRITE_CODE) {
        if (request.size() != 3) return {REQUEST_LENGTH_INVALID_CODE};
		_servo.writeMicroseconds((request[1]<<8) + request[2]);
        return {OK_CODE};
    } else {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

}
