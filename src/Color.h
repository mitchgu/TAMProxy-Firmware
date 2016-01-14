#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include "Adafruit_TCS34725.h"

namespace tamproxy {

class Color : public Device {
private:
  Adafruit_TCS34725 *tcs;
  bool init;
public:
  Color(int integrationTime, int gain);
  ~Color();
  std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};
  
}

#endif
