#include "Port.h"
#include "Tryte.h"

Port& Port::operator<<(Tryte const& input_tryte)
{
	if (this->status_ == Port::Status::WRITE_ONLY || this->status_ == Port::Status::OPEN)
	{
		*t_ = input_tryte;
	}
	return *this;
	
}

Port& Port::operator>>(Tryte& output_tryte)
{
	if (this->status_ == Port::Status::READ_ONLY || this->status_ == Port::Status::OPEN)
	{
		output_tryte = *t_;
	}
	return *this;
}