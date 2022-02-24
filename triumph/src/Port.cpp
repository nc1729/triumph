#include "Port.h"
#include "Tryte.h"

Port& Port::operator<<(Tryte const& input_tryte)
{
	t_ = input_tryte;
	return *this;
}

Port& Port::operator>>(Tryte& output_tryte)
{
	output_tryte = t_;
	return *this;
}