#pragma once

#include "Device.h"
#include "Tryte.h"

#include <iostream>

class Console : public Device
{
private:
	std::istream* in_;
	std::ostream* out_;
public:
	Console() :
		in_{ &std::cin },
		out_{ &std::cout } {};

	Console& operator<<(Tryte const& tryte);
	Console& operator>>(Tryte& tryte);
};