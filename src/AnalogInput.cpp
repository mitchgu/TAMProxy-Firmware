#include "AnalogInput.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {

// Constructor takes a pin number
AnalogInput::AnalogInput(uint8_t pin) {
    _pin = pin;
    pinMode(pin, INPUT);
}

std::vector<uint8_t> AnalogInput::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == ANALOG_INPUT_READ_CODE) {
        if (request.size() != 1) return {REQUEST_LENGTH_INVALID_CODE};
        uint16_t val = analogRead(_pin);
        return {static_cast<uint8_t>(val>>8), static_cast<uint8_t>(val)};
    } else  {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

}
