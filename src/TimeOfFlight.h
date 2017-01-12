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
  bool init;
public:
  TimeOfFlight();
  ~TimeOfFlight();
  std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};
  
}

#endif
