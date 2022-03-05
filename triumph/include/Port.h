#pragma once

#include "Tryte.h"

class Port
{
private:
	Tryte* t_;

public:
	enum class Status
	{
		CLOSED, READ_ONLY, WRITE_ONLY, OPEN
	};
	Status status_;

	Port() : t_{ nullptr }, status_{ Port::Status::CLOSED } {};
	Port(Tryte& target, Port::Status status = Port::Status::OPEN) : 
		t_{&target},
		status_{status}
	{};

	// send Tryte down port
	Port& operator<<(Tryte const& input_tryte);
	// receive Tryte from port
	Port& operator>>(Tryte& output_tryte);
};

