#include "DeviceList.h"
#include "WProgram.h"
#undef min // Arduino libraries define max and min macros which mess with <vector>
#undef max
#include <vector>
#include <cstdint>
#include "Device.h"
#include "config.h"
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "AnalogInput.h"
#include "AnalogOutput.h"
#include "Motor.h"
#include "Encoder.h"
#include "Gyro.h"
#include "Color.h"
#include "Servo.h"

namespace tamproxy {

// Handles a packet request to it
// Capable of adding to, removing from, and clearing the devices
std::vector<uint8_t> DeviceList::handleRequest(std::vector<uint8_t> &request) {
    if (request.size() == 0) { return {REQUEST_LENGTH_INVALID_CODE}; }
    switch(request[0]) {
    case DEVICELIST_ADD_CODE:
        return add(request);
    case DEVICELIST_REMOVE_CODE:
        if (request.size() == 2) {
            return remove(request[1]);
        } else { return {REQUEST_LENGTH_INVALID_CODE}; }
    case DEVICELIST_CLEAR_CODE:
        if (request.size() == 1) {
            clear();
            return {OK_CODE};
        } else { return {REQUEST_LENGTH_INVALID_CODE}; }
    case DEVICELIST_HELLO_CODE:
        // A HELLO request that just responds with a constant
        // Useful when establishing the connection
        return {DEVICELIST_HELLO_CODE};
    default:
        return {REQUEST_BODY_INVALID_CODE};
    }
}

// Gets a device pointer at a certain index of the list.
Device* DeviceList::get(uint8_t idx) {
    if (idx < _devices.size()) {
        return _devices[idx];
    }
    else {
        return nullptr;
    }
}

// Removes a device at a certain index of the list. Returns a status response
std::vector<uint8_t> DeviceList::remove(uint8_t idx) {
    if (idx < _devices.size()) {
        delete _devices[idx];
        _devices[idx] = nullptr;
        return {OK_CODE};
    } else { return {DEVICE_OOR_CODE}; }
}

// Adds a new device to the end of the list. Returns a status response.
// If successful, the response also includes the index of the added device.
std::vector<uint8_t> DeviceList::add(std::vector<uint8_t>& request) {
    Device* d;
    switch(request[1]) {
        case DIGITAL_INPUT_CODE:
            if (request.size() == 4) {
                d = new DigitalInput(request[2], request[3]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
        case DIGITAL_OUTPUT_CODE:
            if (request.size() == 3) {
                d = new DigitalOutput(request[2]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
        case ANALOG_INPUT_CODE:
            if (request.size() == 3) {
                d = new AnalogInput(request[2]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
        case ANALOG_OUTPUT_CODE:
            if (request.size() == 3) {
                d = new AnalogOutput(request[2]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
        case MOTOR_CODE:
            if (request.size() == 4) {
                d = new Motor(request[2], request[3]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
        case ENCODER_CODE:
            if (request.size() == 4) {
                d = new Encoder(request[2], request[3]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
        case GYRO_CODE:
            if (request.size() == 3) {
                d = new Gyro(request[2]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
        case COLOR_CODE:
            if (request.size() == 4) {
                d = new Color(request[2], request[3]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
	    case SERVO_CODE:
		    if (request.size() == 3) {
                d = new Servo(request[2]);
            } else { return {REQUEST_LENGTH_INVALID_CODE}; };
            break;
        default:
            return {DEVICE_INVALID_CODE};
    }
    _devices.push_back(d);
    return {OK_CODE, static_cast<uint8_t>(_devices.size()-1)};
}

// Clears the device list and resets all pins, making sure that the devices get deallocated
void DeviceList::clear() {
    for (uint8_t i = 0; i < NUM_PINS; i++) {
        pinMode(i, INPUT);
    }
    for (Device* d : _devices) {
        if(d != nullptr)
            delete d;
    }
    _devices.clear();
}

}
