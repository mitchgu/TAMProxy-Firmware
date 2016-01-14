#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include "Adafruit_TCS34725.h"

namespace tamproxy {

class Color : public Device {
private:
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
  bool init;
public:
  Color();
  std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};
  
}

#endif
