#ifndef DEVICE_H
#define DEVICE_H

#include <cstdint>

namespace tamproxy {

// Virtual device class that each device should be derived from to take advantage of that sweet, sweet polymorphism
class Device {
public:
    virtual std::vector<uint8_t> handleRequest(std::vector<uint8_t> &request) = 0;
    virtual ~Device() {};
};

}

#endif
