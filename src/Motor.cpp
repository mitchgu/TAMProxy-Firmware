#include "Motor.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {

// Constructor takes a dir pin and a pwm output pin
Motor::Motor(uint8_t dirPin, uint8_t pwmPin) {
    _dirPin = dirPin;
    _pwmPin = pwmPin;
    pinMode(_dirPin, OUTPUT);
    pinMode(_pwmPin, OUTPUT);
    analogWrite(_pwmPin, 0);
}

std::vector<uint8_t> Motor::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == MOTOR_WRITE_CODE) {
        if (request.size() != 4) return {REQUEST_LENGTH_INVALID_CODE};

        digitalWrite(_dirPin, request[1] != 0);
        analogWrite(_pwmPin, (request[2]<<8) + request[3]);
        return {OK_CODE};
    } else {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

}
