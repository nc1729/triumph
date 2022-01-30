#include "PortManager.h"
#include "Device.h"

Device& PortManager::operator[](int64_t const& port)
{
	return _devices.at(port);
}

Device const& PortManager::operator[](int64_t const& port) const
{
	return _devices.at(port);
}

void PortManager::add_device(Device const& device, int64_t const& port)
{
	_devices[port] = device;
}