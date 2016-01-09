#include <cstdint>
#include "WProgram.h"
#undef max // Arduino libraries define max and min macros which mess with <vector>
#undef min
#include <vector>
#include "PacketController.h"
#include "DeviceList.h"
#include "Packet.h"

using namespace tamproxy;

PacketController *pc;
DeviceList *dl;

void setup() {
    analogReadResolution(ADC_RESOLUTION);
    analogWriteResolution(PWM_RESOLUTION);
    analogWriteFrequency(FTM0PIN, PWM0_FREQUENCY);
    analogWriteFrequency(FTM1PIN, PWM1_FREQUENCY);
    analogWriteFrequency(FTM2PIN, PWM2_FREQUENCY);
    pc = new PacketController;
    dl = new DeviceList;
}

void loop() {
    if (pc->receive()) {
        // A new packet has arrived. Retrieve it.
        Packet *latestPacket = pc->getLatestPacket();
        // Pull the destination
        uint8_t dest = latestPacket->dest;
        // Setup a response
        std::vector<uint8_t> response;

        if (dest == DEVICELIST_CODE) {
            // The DeviceList itself should handle this one
            response = dl->handleRequest(latestPacket->payload);
        }
        else {
            // Get the dest device from the DeviceList
            Device* d = dl->get(dest);
            if (d == nullptr) {
                // The dest device doesn't exist
                response = {DEVICE_INVALID_CODE};
            }
            else {
                // Get a response from the dest Device
                response = d->handleRequest(latestPacket->payload);
            }
        }
        // Send a response packet
        pc->transmit(response);
    }
}

int main(void)
{
    setup();
    while(true) { loop(); }
    return 0;
}