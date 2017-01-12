#include "TimeOfFlight.h"
#include <cstdint>
#include "VL53L0X.h"
#include "config.h"

namespace tamproxy {

TimeOfFlight::TimeOfFlight() {
  init = false;
  sensor = new VL53L0X();
  init = sensor->init(true /*use 2V8 mode*/);
  sensor->setTimeout(500);
  sensor->startContinuous();
}

TimeOfFlight::~TimeOfFlight() {
  delete sensor;
}

std::vector<uint8_t> TimeOfFlight::handleRequest(std::vector<uint8_t> &request) {
  if (request.size() != 1) {
    return {REQUEST_LENGTH_INVALID_CODE};
  } else if (request[0] != TOF_READ_CODE) {
    return {REQUEST_BODY_INVALID_CODE};
  } else {
    if (init) {
      uint16_t range = 100;
      range = sensor->readRangeContinuousMillimeters();
      return {static_cast<uint8_t>(range>>8), static_cast<uint8_t>(range)};
    }
    else {
      return {0xff, 0xff};
    }
  }
}

}
