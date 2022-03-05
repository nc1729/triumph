#include "PortManager.h"
#include "Port.h"

Port& PortManager::operator[](Tryte const& port_number)
{
	try
	{
		return ports_.at(port_number);
	}
	catch (std::out_of_range&)
	{
		return dummy;
	}
}

Port* PortManager::connect_port(Tryte const& port_number,
								Port::Status const& port_status)
{
	ports_[Tryte::get_int(port_number)] = Port(port_status);
	return &(ports_[Tryte::get_int(port_number)]);
}

void PortManager::disconnect_port(Tryte const& port_number)
{
	ports_.erase(Tryte::get_int(port_number));
}