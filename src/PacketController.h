#ifndef PACKET_CONTROLLER_H
#define PACKET_CONTROLLER_H

#include <cstdint>
#include <vector>
#include "config.h"
#include "Packet.h"

// A class that handles receiving, parsing, and transmitting packets

namespace tamproxy {

class PacketController {
private:
    uint8_t _receiveBuffer[PACKET_MAX_SIZE];
    uint8_t _receiveBufferHead = 0;
    uint8_t _receiveLength = PACKET_MAX_SIZE;
    bool _midReceive = false;
    bool _errorFlag = false;
    Packet *_latestPacket = new Packet();
    bool processByte(uint8_t byte);
    void transmitError(uint8_t errorCode);
    void decodePacket();
public:
    PacketController();
    bool receive();
    Packet* getLatestPacket();
    void transmit(std::vector<uint8_t> &responseData);
};

}

#endif