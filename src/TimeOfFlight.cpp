#include "TimeOfFlight.h"
#include <cstdint>
#include "VL53L0X.h"
#include <Wire.h>
#include "config.h"

#define ADDRESS_DEFAULT 0b0101001

namespace tamproxy {

// xshutPin specifies the digital pin connected to the xshut control,
// id is a unique identifier 0-15 so that the ToF sensors don't walk
// over each other.
TimeOfFlight::TimeOfFlight(uint8_t xshutPin, uint8_t id) {
  init = false;
  Wire.begin(); // Ensure I2C is set up
  sensor = new VL53L0X();
  _xshutPin = xshutPin;
  _address = (ADDRESS_DEFAULT & 0xf0) | id;
  pinMode(xshutPin, OUTPUT);
  digitalWrite(xshutPin, 0);
  _range = 0xffff;
  _upkeepTimer = millis();
}

TimeOfFlight::~TimeOfFlight() {
  delete sensor;
}

std::vector<uint8_t> TimeOfFlight::handleRequest(std::vector<uint8_t> &request) {
  if (request.size() != 1) {
    return {REQUEST_LENGTH_INVALID_CODE};
  } else if (request[0] == TOF_ENABLE_CODE) {
    digitalWrite(_xshutPin, 1); // re-enable the sensor
    delay(100);
    sensor->setAddress(_address);
    bool isinit = sensor->init(false);
    sensor->setTimeout(500);
    sensor->startContinuous();
    init = isinit;
    return {_address};
  } else if (request[0] == TOF_READ_CODE) {
    return {static_cast<uint8_t>(_range>>8), static_cast<uint8_t>(_range)};
  } else {
    return {REQUEST_BODY_INVALID_CODE};
  }
}

void TimeOfFlight::doUpkeep() {
  // Only upkeep every 10ms
  if (millis() - _upkeepTimer <= 10) {
    return;
  }
  _upkeepTimer = millis();
  
  // Time of flight sensor should continuously trigger reads and cache them
  // otherwise we get really slow throughput while firmware blocks on a single
  // distance read.
  if (init) {
    uint16_t range = 0;
    range = sensor->readRangeContinuousNoBlockMillimeters();
    if (range != 65535) {
      // Cache result to pass to pyHost
      _range = range;
    }
  }
}

}
