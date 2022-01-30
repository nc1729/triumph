#pragma once

#include "Device.h"
#include "Tryte.h"

#include <iostream>

class Console : public Device
{
private:
	std::istream* _in;
	std::ostream* _out;
public:
	Console() :
		_in{ &std::cin },
		_out{ &std::cout } {};

	Console& operator<<(Tryte const& tryte);
	Console& operator>>(Tryte& tryte);
};