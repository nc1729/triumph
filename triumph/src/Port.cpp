#include "Port.h"
#include "Tryte.h"

Port& Port::operator<<(Tryte const& input_tryte)
{
	if (this->status == Port::Status::OPEN)
	{
		// only modify Port if Port is OPEN
		t_ = input_tryte;
	}
	return *this;
	
}

Port& Port::operator>>(Tryte& output_tryte)
{
	if (this->status != Port::Status::CLOSED)
	{
		// if Port is OPEN or READ_ONLY, allow read access
		output_tryte = t_;
	}
	return *this;
}