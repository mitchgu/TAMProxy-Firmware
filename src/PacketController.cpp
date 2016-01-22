#include "PacketController.h"
#include "WProgram.h"
#include <cstdint>
#undef min // Arduino libraries define max and min macros which mess with <vector>
#undef max
#include <vector>
#include "config.h"
#include "Packet.h"

namespace tamproxy {

// Constructor
PacketController::PacketController() {
    Serial.begin(BAUD_RATE);    // Setup the serial port
    Serial.setTimeout(0);       // Don't block the program if the serial buffer is empty
}

// Receiver. Reads from the serial buffer until there is nothing left or it has parsed a packet
// Returns true if a packet was found, else false
bool PacketController::receive() {
    int newByte;
    while (true) {
        newByte = Serial.read(); // Read a byte from the buffer
        switch (newByte) {
        case -1:
            // Serial buffer is empty, wait till next call to resume
            return false;
        case PACKET_START_BYTE:
            // Prepare to read a new packet into the receiveBuffer
            if (!_midReceive) {
                // We weren't in a packet, start a new one
                _receiveBufferHead = 0;
                _midReceive = true;
                _errorFlag = false;
                _receiveLength = PACKET_MAX_SIZE;
            }
            return processByte(newByte);
        default:
            if (!_errorFlag) {
                if (_midReceive) {
                    return processByte(newByte);
                } else {
                    // A packet wasn't started yet and this byte is in no man's land
                    transmitError(NO_MANS_LAND_CODE); 
                    return false;
                }
            } else { return false; }
        }
    }
}

bool PacketController::processByte(uint8_t byte) {
    // Add the new byte to our buffer
    _receiveBuffer[_receiveBufferHead] = byte;
    if (_receiveBufferHead == 3) {
        // Time to read the length byte
        if (byte > PACKET_MAX_SIZE) { 
            // Specified length is too long
            transmitError(PACKET_OOR_CODE);
        } else if (byte < PACKET_MIN_RESPONSE_SIZE) {
            transmitError(PACKET_SHORT_CODE);
        }
        else { _receiveLength = byte; }
    }
    if (_receiveBufferHead == _receiveLength - 1) {
        // We've reached the last byte
        decodePacket();
        _midReceive = false;
        return true;
    }
    _receiveBufferHead++;
    return false;
}

void PacketController::decodePacket() {
    _latestPacket->id = _receiveBuffer[1] + (_receiveBuffer[2] << 8);
    _latestPacket->dest = _receiveBuffer[4];
    _latestPacket->payload.assign(_receiveBuffer + 5, _receiveBuffer + _receiveBufferHead + 1);
}

// Returns the last processed packet
Packet* PacketController::getLatestPacket() {
    return _latestPacket;
}

// Transmitter. Given a response vector, transmit a response packet.
// The packet has the same ID as the last received packet.
void PacketController::transmit(std::vector<uint8_t> &responseData) {
    uint16_t id = _latestPacket->id;
    uint8_t header[4] = {
        PACKET_START_BYTE,
        static_cast<uint8_t>(id),
        static_cast<uint8_t>(id>>8),
        static_cast<uint8_t>(4 + responseData.size())
    };

    Serial.write(header, 4);
    Serial.write(&responseData[0], responseData.size());

    Serial.send_now();
}

// Private function to transmit errors in parsing packets
// Since a packet couldn't be read, the ID is fixed to PACKET_ERROR_ID
void PacketController::transmitError(uint8_t errorCode) {
    Serial.write(errorCode);
    _errorFlag = true;
    _midReceive = false;
}

}
