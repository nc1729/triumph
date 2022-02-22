#include "PortManager.h"
#include "Device.h"

Device& PortManager::operator[](int64_t const& port)
{
	return devices_.at(port);
}

Device const& PortManager::operator[](int64_t const& port) const
{
	return devices_.at(port);
}

void PortManager::add_device(Device const& device, int64_t const& port)
{
	devices_[port] = device;
}