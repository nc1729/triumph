#include <istream>
#include <cstdint>
#include <deque>

#include "Tryte.h"
#include "Port.h"
#include "PortManager.h"
#include "Keyboard.h"

void Keyboard::scan()
{
	Tryte input;
	if (input_stream_ >> input)
	{
		input_buffer_.push_front(input);
	}	
}

void Keyboard::push()
{
	if (input_buffer_.size() != 0)
	{
		data_ = input_buffer_.back();
		data_port_->status = Port::Status::FULL;
		input_buffer_.pop_back();
	}
}

void Keyboard::cycle()
{
	scan();
	push();
}