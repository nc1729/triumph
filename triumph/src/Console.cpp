#include <iostream>

#include "Bank.h"
#include "Console.h"

void Console::flush_to_out()
{
	// set console status to busy
	buffer[CONSOLE_STATE_ADDR + 9841][STATUS_FLAG] = 0;

	// get number of characters to print
	int64_t output_size = Tryte::get_int(buffer[COUT_BUFFER_LEN + 9841]);
	if (output_size < 0)
	{
		// a negative number of characters makes no sense, so do nothing
		output_size = 0;
	}

	// print to output
	for (int64_t i = 0; i < output_size; i++)
	{
		Tryte& tryte = buffer[COUT_BUFFER_START + 9841 + i];

		if (mode == DisplayMode::CHAR)
		{
			// print the Tryte's ASCII char equivalent
			out << static_cast<char>(Tryte::get_int(tryte));
		}
		else if (mode == DisplayMode::INTEGER)
		{
			out << Tryte::get_int(tryte);
		}
		else
		{
			out << tryte;
		}
	}

	// set buffer length to 0 again
	buffer[COUT_BUFFER_LEN + 9841] = 0;

	// set console status to ready
	buffer[CONSOLE_STATE_ADDR + 9841][STATUS_FLAG] = 1;
}

void Console::read_from_in()
{
	// set console status to busy
	buffer[CONSOLE_STATE_ADDR + 9841][STATUS_FLAG] = 0;

	int64_t input_size = 0;
	// read from input
	Tryte temp;
	while (in >> temp && input_size < 729)
	{
		buffer[CIN_BUFFER_START + input_size] = temp;
		input_size++;
	}

	// set number of chars read for CPU
	buffer[CIN_BUFFER_LEN] = input_size;

	// set console status to ready
	buffer[CONSOLE_STATE_ADDR + 9841][STATUS_FLAG] = 1;
}