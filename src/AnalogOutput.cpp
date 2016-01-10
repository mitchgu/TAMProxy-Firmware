#include "AnalogOutput.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {

// Constructor takes a pin number and whether it's pullup
AnalogOutput::AnalogOutput(uint8_t pin) {
    _pin = pin;
    pinMode(pin, OUTPUT);
}

std::vector<uint8_t> AnalogOutput::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == ANALOG_OUTPUT_WRITE_CODE) {
        if (request.size() != 3) return {REQUEST_LENGTH_INVALID_CODE};

        analogWrite(_pin, (request[1]<<8) + request[2]);
        return {OK_CODE};
    } else {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

}
