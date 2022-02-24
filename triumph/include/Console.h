#pragma once

#include <deque>

#include "Tryte.h"

class Console
{
private:
	std::deque<Tryte> input_buffer_;
	std::deque<Tryte> output_buffer_;
	// send 
	Port* input_port_;
	Port* output_port_;
	Port* status_port_;
};