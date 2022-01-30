#pragma once

#include "Device.h"

#include <map>

/*
PortManager class
- Wrapper around Devices and their port numbers. For the CPU to talk to a device, it must be in this list.
*/
class PortManager
{
private:
    std::map<int64_t, Device> _devices;
public:
    Device& operator[](int64_t const& port);
    Device const& operator[](int64_t const& port) const;

    void add_device(Device const& device, int64_t const& port);
};