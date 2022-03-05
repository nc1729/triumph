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
		STREAM_STATE_TRIT_ = 1;
	}
	else
	{
		// something went wrong with the stream, tell the CPU this
		STREAM_STATE_TRIT_ = -1;
	}
	
}

void Keyboard::push()
{
	if (input_buffer_.size() != 0)
	{
		data_ = input_buffer_.back();
		INPUT_READY_TRIT_ = 1;
	}
	else
	{
		// port points to nothing, tell the CPU this
		INPUT_READY_TRIT_ = -1;
	}
}

void Keyboard::cycle()
{
	scan();
	push();
}