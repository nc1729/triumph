#pragma once

#include "Tryte.h"

class Port
{
private:
	Tryte t_;

public:
	enum class Status
	{
		CLOSED, READ_ONLY, WRITE_ONLY, OPEN
	};
	Status status;

	Port() : status{ Port::Status::OPEN } {};
	Port(Port::Status status) : status{ status } {};

	// send Tryte down port
	Port& operator<<(Tryte const& input_tryte);
	// receive Tryte from port
	Port& operator>>(Tryte& output_tryte);

	// public access to Tryte (circumventing Port status)
	Tryte& tryte();
};

