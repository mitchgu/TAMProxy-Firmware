#include "DigitalInput.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {

// Constructor takes a pin number and whether it's pullup
DigitalInput::DigitalInput(uint8_t pin, uint8_t pullup) {
    _pin = pin;
    if (pullup) {
        pinMode(pin, INPUT_PULLUP);
    } else {
        pinMode(pin, INPUT);
    }
}

std::vector<uint8_t> DigitalInput::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == DIGITAL_INPUT_READ_CODE) {
        if (request.size() != 1) return {REQUEST_LENGTH_INVALID_CODE};

        return {digitalRead(_pin)};
    } else {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

}
