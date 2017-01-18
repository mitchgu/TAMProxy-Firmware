#ifndef TIME_OF_FLIGHT_H
#define TIME_OF_FLIGHT_H

#include <cstdint>
#include <vector>
#include "Device.h"
#include "VL53L0X.h"

namespace tamproxy {

class TimeOfFlight : public Device {
private:
  VL53L0X *sensor;
  uint8_t _xshutPin;
  uint8_t _address;
  bool init;
public:
  TimeOfFlight(uint8_t xshutPin, uint8_t id);
  ~TimeOfFlight();
  std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};
  
}

#endif
