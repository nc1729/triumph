#include <iostream>
#include <string>
#include <sstream>

#include "Bank.h"
#include "Console.h"

void Console::flush_to_out()
{
	// set console status to busy
	buffer[CONSOLE_STATE_ADDR + 9841][STATUS_FLAG] = 0;

	if (mode == DisplayMode::CHAR)
	{
		// if in char mode, no need to set output buffer size
		// just print until we encounter a '\0' char
		// obviously stop if we hit the end of the buffer though
		int64_t i = 0;
		Tryte& tryte = buffer[COUT_BUFFER_START + 9841 + i];
		while (tryte != Tryte(0) && i < BUFFER_SIZE)
		{
			// print the tryte's ASCII char equivalent
			out << static_cast<char>(tryte);
		}
	}
	else
	{
		// get number of Trytes to print
		int64_t output_size = Tryte::get_int(buffer[COUT_BUFFER_LEN + 9841]);
		if (output_size < 0)
		{
			// a negative number of characters makes no sense, so do nothing
			output_size = 0;
		}
		else if (output_size > BUFFER_SIZE)
		{
			// if a larger number of Trytes is required for printing, prevent
			// output of Trytes outside of output buffer by capping it here
			output_size = BUFFER_SIZE;
		}

		// print to output
		for (int64_t i = 0; i < output_size; i++)
		{
			Tryte& tryte = buffer[COUT_BUFFER_START + 9841 + i];

			if (mode == DisplayMode::INTEGER)
			{
				out << Tryte::get_int(tryte);
			}
			else
			{
				out << tryte;
			}
		}
	}

	// set buffer length to 0 again
	buffer[COUT_BUFFER_LEN + 9841] = 0;

	// set console status to ready
	buffer[CONSOLE_STATE_ADDR + 9841][STATUS_FLAG] = 1;
}

int64_t Console::read_chars(std::string const& input_string)
{
	int64_t input_size = 0;
	Tryte temp;
	for (char c : input_string)
	{
		temp = static_cast<int64_t>(c);
		buffer[CIN_BUFFER_START + 9841 + input_size] = temp;
		input_size++;
		if (input_size > BUFFER_SIZE - 2)
		{
			// append a \'0' char to the end of any read string
			// in this case, the string is 729 Trytes with the '\0' (filled the buffer)
			buffer[CIN_BUFFER_START + 9841 + BUFFER_SIZE - 1] = temp;
			return BUFFER_SIZE;
		}
	}
	// append a \'0' char to the end of any read string
	buffer[CIN_BUFFER_START + 9841 + input_size] = temp;
	input_size++;
	// input_size isn't necessary for strings with '\0' endings, but good to have anyway
	return input_size;
}

int64_t Console::read_ints(std::string const& input_string)
{
	std::stringstream input(input_string);
	int64_t input_size = 0;
	int64_t input_int = 0;
	Tryte temp;
	// work through line of input, looking for valid integers
	// when we hit the end of input or find something that can't be converted to int,
	// stop filling the input buffer
	while (input >> input_int)
	{
		// input was a valid int
		temp = input_int;
		buffer[CIN_BUFFER_START + 9841 + input_size] = temp;
		input_size++;
		if (input_size > BUFFER_SIZE - 1)
		{
			return BUFFER_SIZE;
		}
	}
	return input_size;
}

int64_t Console::read_trytes(std::string const& input_string)
{
	std::stringstream input(input_string);
	int64_t input_size = 0;
	Tryte temp;
	// work through line of input, looking for valid Trytes (in septavingtemsal form)
	// when we hit the end of input or find something that can't be converted to a Tryte,
	// stop filling the input buffer
	while (input >> temp)
	{
		// input was a valid Tryte
		buffer[CIN_BUFFER_START + 9841 + input_size] = temp;
		input_size++;
		if (input_size > BUFFER_SIZE - 1)
		{
			return BUFFER_SIZE;
		}
	}
	return input_size;
}

void Console::read_from_in()
{
	// set console status to busy
	buffer[CONSOLE_STATE_ADDR + 9841][STATUS_FLAG] = 0;

	int64_t input_size = 0;
	// read from input
	std::string input_string;
	std::getline(in, input_string);
	if (mode == DisplayMode::CHAR)
	{
		input_size = read_chars(input_string);
		
	}
	else if (mode == DisplayMode::INTEGER)
	{
		input_size = read_ints(input_string);
	}
	else
	{
		input_size = read_trytes(input_string);
	}

	// set number of chars read for CPU
	buffer[CIN_BUFFER_LEN + 9841] = input_size;

	// reset input stream for next read
	in.clear();

	// set console status to ready
	buffer[CONSOLE_STATE_ADDR + 9841][STATUS_FLAG] = 1;
}

void Console::set_display_mode()
{
	// display mode Tryte
	int64_t display_mode = Tryte::get_int(buffer[CONSOLE_DISPLAY_MODE + 9841]);
	if (display_mode == 0)
	{
		mode = DisplayMode::RAW;
	}
	else if (display_mode == 1)
	{
		mode = DisplayMode::INTEGER;
	}
	else if (display_mode == 2)
	{
		mode = DisplayMode::CHAR;
	}
}