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
	// data port for sending Trytes to CPU
	Port* data_port_;
	// data Tryte exposed to CPU
	Tryte data_ = 0;
	// status port for communicating Keyboard status to CPU
	Port* status_port_;
	// Tryte to hold internal Keyboard status, exposed to CPU
	Tryte status_ = 0;
	// input_stream (by default, std::cin)
	std::istream& input_stream_;
	// queue for buffered input
	std::deque<Tryte> input_buffer_;

	
	// +1 if port not empty, -1 if port is empty, 0 if uninitialised
	int8_t& INPUT_READY_TRIT_ = status_[0];
	// +1 if stream is good, -1 if stream is bad, 0 if uninitialised
	int8_t& STREAM_STATE_TRIT_ = status_[1];

	// scan input_stream for input, and add it to the input_buffer
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
		data_port_{ports.connect_port(data_, data_port_num, Port::Status::READ_ONLY)},
		status_port_{ports.connect_port(status_, status_port_num, Port::Status::READ_ONLY)}
	{};

	// scan for input and push to data port (to run in separate thread)
	void cycle();
};