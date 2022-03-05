

#include "ConsoleOut.h"

void ConsoleOut::cycle()
{
	if (data_port_->status == Port::Status::FULL)
	{
		output_buffer_.push_front(data_);
		data_port_->status = Port::Status::EMPTY;
	}
}
void ConsoleOut::flush()
{
	if (FLUSH_TRIT_ == 1 && output_buffer_.size() > 0)
	{
		// loop over output buffer back to front - print the oldest things in the buffer first
		for (auto it = output_buffer_.rbegin(); it != output_buffer_.rend(); it++)
		{
			output_stream_ << *it;
		}

		// now clear the buffer
		output_buffer_.clear();

		// and set FLUSH_TRIT to 0, otherwise console will try to flush again
		FLUSH_TRIT_ = 0;
	}
}