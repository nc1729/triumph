#pragma once
#include <iostream>
#include <string>

#include "Bank.h"

class Console
{
private:
	enum class DisplayMode
	{
		RAW,
		INTEGER,
		CHAR
	};
	std::ostream& out;
	std::istream& in;

	int64_t read_chars(std::string const& input_string);
	int64_t read_ints(std::string const& input_string);
	int64_t read_trytes(std::string const& input_string);
public:
	Console(std::ostream& out, std::istream& in) : out{ out }, in{ in } {};
	DisplayMode mode = DisplayMode::RAW;
	int64_t static constexpr CONSOLE_BANK = -1;
	int64_t static constexpr BUFFER_SIZE = 729;
	Bank buffer{ CONSOLE_BANK };

	// useful memory addresses
	static int64_t constexpr COUT_BUFFER_START = -9841; // $MMM
	static int64_t constexpr CIN_BUFFER_START = -9841 + BUFFER_SIZE; // $LMM
	static int64_t constexpr CONSOLE_BANK_END = -9841 + 6561; // $DMM
	// CPU needs to tell the console how many Trytes to print
	// this is stored at this address - reset to 0 after a flush
	static int64_t constexpr COUT_BUFFER_LEN = CONSOLE_BANK_END - 4; // $Emj
	// CPU can tell how many Trytes are stored in input buffer here
	// overwritten when another read from stdin happens
	static int64_t constexpr CIN_BUFFER_LEN = CONSOLE_BANK_END - 3; // $Emk
	static int64_t constexpr CONSOLE_DISPLAY_MODE = CONSOLE_BANK_END - 2; // $Eml
	static int64_t constexpr CONSOLE_STATE_ADDR = CONSOLE_BANK_END - 1; // $Emm

	/*
	console status trits - memory at $Emm 
	*/
	// console status flag
	// + : console is ready for input/output
	// 0 : console is busy (printing or waiting for input)
	// - : console error
	static size_t constexpr STATUS_FLAG = 0;
	// out request flag
	// + : flush buffer to out stream when ready
	// 0 : do nothing
	// - : do nothing
	static size_t constexpr OUT_REQUEST_FLAG = 1;
	// in request flag
	// + : fetch input from in stream
	// 0 : do nothing
	// - : do nothing
	static size_t constexpr IN_REQUEST_FLAG = 2;

	/*
	console operations
	*/
	void flush_to_out();
	void read_from_in();
	void set_display_mode();
};