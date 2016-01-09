#include "Encoder.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"

namespace tamproxy {

// Constructor takes a pin number
Encoder::Encoder(uint8_t pinA, uint8_t pinB) : _pinA(pinA), _pinB(pinB), _enc(pinA, pinB) {
}

std::vector<uint8_t> Encoder::handleRequest(std::vector<uint8_t> &request) {
    if (request[0] == ENCODER_READ_CODE) {
        if (request.size() != 1) return {REQUEST_LENGTH_INVALID_CODE};

        // library is bad, should use unsigned for well-defined overflow
        uint32_t val = static_cast<uint32_t>(_enc.read());
        return {
            static_cast<uint8_t>(val>>24),
            static_cast<uint8_t>(val>>16),
            static_cast<uint8_t>(val>>8),
            static_cast<uint8_t>(val)
        };
    } else if (request[0] == ENCODER_WRITE_CODE) {
        if (request.size() != 5) return {REQUEST_LENGTH_INVALID_CODE};

        uint32_t val = (request[1] << 24)
                     | (request[2] << 16)
                     | (request[1] << 8)
                     | (request[0] << 0);

        // again, should be unsigned, but we have to cast
        _enc.write(static_cast<int32_t>(val));
        return {OK_CODE};
    } else {
        return {REQUEST_BODY_INVALID_CODE};
    }
}

}
