#ifndef DEVICE_LIST_H
#define DEVICE_LIST_H

#include <cstdint>
#include <vector>
#include "Device.h"

namespace tamproxy {

// A class to manage the list of active devices
class DeviceList : public Device {
private:
    std::vector<Device*> _devices;
    std::vector<uint8_t> remove(uint8_t idx);
    std::vector<uint8_t> add(std::vector<uint8_t> &request);
    void clear();
public:
    std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request);
    Device* get(uint8_t idx);
};

}
#endif