#include "Port.h"
#include "Tryte.h"

Port& Port::operator<<(Tryte const& input_tryte)
{
	if (this->status == Port::Status::EMPTY)
	{
		*t_ = input_tryte;
		this->status = Port::Status::FULL;
	}
	return *this;
	
}

Port& Port::operator>>(Tryte& output_tryte)
{
	if (this->status == Port::Status::FULL)
	{
		output_tryte = *t_;
		this->status = Port::Status::EMPTY;
	}
	return *this;
}