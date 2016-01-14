#include "Color.h"
#include <cstdint>
#include "Adafruit_TCS34725.h"
#include "config.h"

namespace tamproxy {

Color::Color() {
  init = tcs.begin();
}

std::vector<uint8_t> Color::handleRequest(std::vector<uint8_t> &request) {
  if (request.size() != 1) {
    return {REQUEST_LENGTH_INVALID_CODE};
  } else if (request[0] != COLOR_READ_CODE) {
    return {REQUEST_BODY_INVALID_CODE};
  } else {
    uint16_t r, g, b, c, colorTemp, lux;
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);

    // Return r, g, b, c, plus some extra info
    // Total: 12 bytes (6 uint16_t)
    if (init) {
      return {static_cast<uint8_t>((r >> 8) & 0xff),
          static_cast<uint8_t>(r & 0xff),
          static_cast<uint8_t>((g >> 8) & 0xff),
          static_cast<uint8_t>(g & 0xff),
          static_cast<uint8_t>((b >> 8) & 0xff),
          static_cast<uint8_t>(b & 0xff),
          static_cast<uint8_t>((c >> 8) & 0xff),
          static_cast<uint8_t>(c & 0xff),
          static_cast<uint8_t>((colorTemp >> 8) & 0xff),
          static_cast<uint8_t>(colorTemp & 0xff),
          static_cast<uint8_t>((lux >> 8) & 0xff),
          static_cast<uint8_t>(lux & 0xff)};
    }
    else {
      return {0xff, 0xff, 0xff, 0xff,
          0xff, 0xff, 0xff, 0xff,
          0xff, 0xff, 0xff, 0xff};
    }
  }
}

}
