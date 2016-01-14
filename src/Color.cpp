#include "Color.h"
#include <cstdint>
#include "Adafruit_TCS34725.h"
#include "config.h"

namespace tamproxy {

Color::Color(int integrationTime, int gain) {
  init = false;
  tcs34725IntegrationTime_t it;
  if (integrationTime == 1) {
    it = TCS34725_INTEGRATIONTIME_2_4MS;
  }
  else if (integrationTime == 2) {
    it = TCS34725_INTEGRATIONTIME_24MS;
  }
  else if (integrationTime == 3) {
    it = TCS34725_INTEGRATIONTIME_50MS;
  }
  else if (integrationTime == 4) {
    it = TCS34725_INTEGRATIONTIME_101MS;
  }
  else if (integrationTime == 5) {
    it = TCS34725_INTEGRATIONTIME_154MS;
  }
  else if (integrationTime == 6) {
    it = TCS34725_INTEGRATIONTIME_700MS;
  }
  else {
    return;
  }

  tcs34725Gain_t g;
  if (gain == 1) {
    g = TCS34725_GAIN_1X;
  }
  else if (gain == 2) {
    g = TCS34725_GAIN_4X;
  }
  else if (gain == 3) {
    g = TCS34725_GAIN_16X;
  }
  else if (gain == 4) {
    g = TCS34725_GAIN_60X;
  }
  else {
    return;
  }
  
  tcs = new Adafruit_TCS34725(it, g);
  init = tcs->begin();
}

Color::~Color() {
  delete tcs;
}

std::vector<uint8_t> Color::handleRequest(std::vector<uint8_t> &request) {
  if (request.size() != 1) {
    return {REQUEST_LENGTH_INVALID_CODE};
  } else if (request[0] != COLOR_READ_CODE) {
    return {REQUEST_BODY_INVALID_CODE};
  } else {
    uint16_t r, g, b, c, colorTemp, lux;
    tcs->getRawData(&r, &g, &b, &c);
    colorTemp = tcs->calculateColorTemperature(r, g, b);
    lux = tcs->calculateLux(r, g, b);

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
