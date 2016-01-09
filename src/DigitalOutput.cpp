#include "DigitalOutput.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {

// Constructor takes a pin number and whether it's pullup
DigitalOutput::DigitalOutput(uint8_t pin) {
    _pin = pin;
    pinMode(pin, OUTPUT);
}

std::vector<uint8_t> DigitalOutput::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == DIGITAL_OUTPUT_WRITE_CODE)  {
        if (request.size() != 2) return {REQUEST_LENGTH_INVALID_CODE};

        digitalWrite(_pin, request[1] != 0);
        return {OK_CODE};
    } else {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

}
