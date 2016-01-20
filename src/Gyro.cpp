#include "Gyro.h"
#include <cstdint>
#include "WProgram.h"
#include "Device.h"
#include "config.h"
#include "SPI.h"

namespace tamproxy {

Gyro::Gyro(uint8_t sspin) {
  _sspin = sspin;
  pinMode(sspin, OUTPUT);
  digitalWrite(sspin, HIGH);
  SPI.begin();

  // Prime the device with an initial read word
  SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));
  // Pull sspin low per 32-bit word communication
  digitalWrite(_sspin, LOW);
  // Send read word
  SPI.transfer((READ_WORD >> 24) & 0xff);
  SPI.transfer((READ_WORD >> 16) & 0xff);
  SPI.transfer((READ_WORD >> 8) & 0xff);
  SPI.transfer((READ_WORD >> 0) & 0xff);
  digitalWrite(_sspin, HIGH);
  SPI.endTransaction();
}

std::vector<uint8_t> Gyro::handleRequest(std::vector<uint8_t> &request) {
  if (request.size() < 1) {
    return {REQUEST_LENGTH_INVALID_CODE};
  } else if (request[0] == GYRO_CALIBRATE_CODE) {
    if (request.size() != 3) {
      return {REQUEST_LENGTH_INVALID_CODE};
    }
    // Assemble 32-bit word to send, with DNC register address and 16-bit value
    uint32_t writeWord = WRITE_DNC_WORD;
    uint16_t val = static_cast<uint16_t>(request[1]) << 8 | static_cast<uint16_t>(request[2]);
    writeWord |= static_cast<uint32_t>(val) << 1;
    // Compute and set parity bit
    int parity = 0;
    for (int i = 0; i < 32; ++i) {
      if ((writeWord >> i) & 0b1) {
        parity = !parity;
      }
    }
    // We want odd total parity
    writeWord |= ((!parity) & 0b1);
    
    // Send the word
    SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_sspin, LOW);
    uint8_t b1 = SPI.transfer((writeWord >> 24) & 0xff);
    uint8_t b2 = SPI.transfer((writeWord >> 16) & 0xff);
    uint8_t b3 = SPI.transfer((writeWord >> 8) & 0xff);
    uint8_t b4 = SPI.transfer((writeWord >> 0) & 0xff);
    digitalWrite(_sspin, HIGH);
    SPI.endTransaction();

    return {b1, b2, b3, b4};
  } else if (request[0] == GYRO_READ_CODE) {
    if (request.size() != 1) {
      return {REQUEST_LENGTH_INVALID_CODE};
    }
    // Send read word, and store response from previous read
    SPI.beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_sspin, LOW);
    uint8_t b1 = SPI.transfer((READ_WORD >> 24) & 0xff);
    uint8_t b2 = SPI.transfer((READ_WORD >> 16) & 0xff);
    uint8_t b3 = SPI.transfer((READ_WORD >> 8) & 0xff);
    uint8_t b4 = SPI.transfer((READ_WORD >> 0) & 0xff);
    digitalWrite(_sspin, HIGH);
    SPI.endTransaction();

    // Return bytes of reading, MSB first
    return {b1, b2, b3, b4};
  } else {
    return {REQUEST_BODY_INVALID_CODE};
  }
}

}
