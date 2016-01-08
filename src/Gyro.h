#ifndef GYRO_H
#define GYRO_H

#include <cstdint>
#include <vector>
#include "Device.h"

class Gyro : public Device {
private:
  uint8_t _sspin;
  static const uint32_t CLOCK_SPEED = 2000000; // Clocked at 2 MHz
  static const uint32_t READ_WORD = 0x20000000;
public:
  Gyro(uint8_t sspin);
  std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
};

#endif
