#include "AnalogInput.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

// Constructor takes a pin number
AnalogInput::AnalogInput(uint8_t pin) {
    _pin = pin;
    pinMode(pin, INPUT);
}

// The request must have just 'G' (get) to go through. Kinda just a formality
std::vector<uint8_t> AnalogInput::handleRequest(std::vector<uint8_t> &request) {
    if (request.size() != 1) {
        return {REQUEST_LENGTH_INVALID_CODE};
    } else if (request[0] != ANALOG_INPUT_READ_CODE) {
        return {REQUEST_BODY_INVALID_CODE};
    } else {
        uint16_t val = analogRead(_pin);
        return {static_cast<uint8_t>(val>>8), static_cast<uint8_t>(val)};
    }
}