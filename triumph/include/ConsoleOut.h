#pragma once

#include <ostream>
#include <deque>

#include "Tryte.h"
#include "Port.h"
#include "PortManager.h"

class ConsoleOut
{
private:
    	// output stream (by default, std::cout)
	std::ostream& output_stream_;
	// data port for receiving Trytes from CPU
	Port* data_port_;
	// data Tryte exposed to CPU
	Tryte data_ = 0;
	// queue for buffered output
	std::deque<Tryte> output_buffer_;

	// status port for communicating with CPU
	Port* status_port_;
	// Tryte to hold internal Console status, exposed to CPU
	Tryte status_ = 0;
	// +1 to flush stream (must be triggered by program!)
	int8_t& FLUSH_TRIT_ = status_[0];

public:
	ConsoleOut(PortManager& ports,
		int64_t data_port_num,
		int64_t status_port_num,
		std::ostream& output_stream = std::cout) :
		output_stream_{ output_stream },
		data_port_{ ports.connect_port(data_, data_port_num, Port::Status::EMPTY) },
		status_port_{ ports.connect_port(status_, status_port_num, Port::Status::EMPTY) }
	{};

	// get a single Tryte from the data port and add it to the output buffer
	void cycle();

	// flush the output buffer
	void flush();
};
