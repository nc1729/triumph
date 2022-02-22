#include "Console.h"
#include "Device.h"
#include "Tryte.h"

Console& Console::operator<<(Tryte const& tryte)
{
	*out_ << Tryte::get_int(tryte);
	return *this;
}

Console& Console::operator>>(Tryte& tryte)
{
	*in_ >> tryte;
	return *this;
}