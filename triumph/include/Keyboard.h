#pragma once

#include <istream>
#include <cstdint>
#include <deque>

#include "Tryte.h"
#include "Port.h"
#include "PortManager.h"

class Keyboard
{
private:
    	// input stream (by default, std::cin)
	std::istream& input_stream_;
	// data port for sending Trytes to CPU
	Port* data_port_;
	// data Tryte exposed to CPU
	Tryte data_ = 0;
	// queue for buffered input
	std::deque<Tryte> input_buffer_;

	// status port for communicating Keyboard status to CPU
	Port* status_port_;
	// Tryte to hold internal Keyboard status, exposed to CPU
	Tryte status_ = 0;

	// scan input_stream for input, and add it to the input buffer
	void scan();

	// push next Tryte in input_buffer to port, so CPU can use it
	void push();


public:
	Keyboard() = delete;
	Keyboard(PortManager& ports,
		     int64_t data_port_num,
		     int64_t status_port_num,
			 std::istream& input_stream = std::cin) :
		input_stream_{input_stream},
		data_port_{ports.connect_port(data_, data_port_num, Port::Status::EMPTY)},
		status_port_{ports.connect_port(status_, status_port_num, Port::Status::EMPTY)}
	{};

	// scan for input and push to data port (to run in separate thread)
	void cycle();
};
