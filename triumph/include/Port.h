#pragma once

#include "Tryte.h"

class Port
{
private:
	Tryte t_;

public:
	enum class Status
	{
		CLOSED = -1, READ_ONLY = 0, OPEN = 1
	};
	Status status;

	Port() : status{ Port::Status::OPEN } {};
	Port(Port::Status status) : status{ status } {};

	// send Tryte down port
	Port& operator<<(Tryte const& input_tryte);
	// receive Tryte from port
	Port& operator>>(Tryte& output_tryte);
};

