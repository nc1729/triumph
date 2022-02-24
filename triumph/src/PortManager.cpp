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

Port const& PortManager::operator[](Tryte const& port_number) const
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

void PortManager::connect_port(Tryte const& port_number)
{
	ports_[Tryte::get_int(port_number)] = Port();
}

void PortManager::disconnect_port(Tryte const& port_number)
{
	ports_.erase(Tryte::get_int(port_number));
}