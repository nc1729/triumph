#include "Console.h"
#include "Device.h"
#include "Tryte.h"

Console& Console::operator<<(Tryte const& tryte)
{
	*_out << Tryte::get_int(tryte);
	return *this;
}

Console& Console::operator>>(Tryte& tryte)
{
	*_in >> tryte;
	return *this;
}