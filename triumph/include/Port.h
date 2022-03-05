#pragma once

#include "Tryte.h"

class Port
{
private:
	Tryte* t_;

public:
	enum class Status
	{
		// port won't accept any input and should not be read from
		CLOSED,
		// port is ready for input
		EMPTY,
		// port has received input and is ready to send
		FULL
	};
	Status status;

	Port() : t_{ nullptr }, status{ Port::Status::CLOSED } {};
	Port(Tryte& target, Port::Status status = Port::Status::EMPTY) : 
		t_{&target},
		status{status}
	{};

	// send Tryte down port
	Port& operator<<(Tryte const& input_tryte);
	// receive Tryte from port
	Port& operator>>(Tryte& output_tryte);
};

