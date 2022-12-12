#pragma once
#include <iostream>
#include <string>

#include "common/constants.h"
#include "Memory/MemoryBlock.h"

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

	Tryte read_chars(std::string const& input_string);
	Tryte read_ints(std::string const& input_string);
	Tryte read_trytes(std::string const& input_string);
public:
	Console(std::ostream& out, std::istream& in) : out{ out }, in{ in } {};
	DisplayMode mode = DisplayMode::RAW;
	int64_t static constexpr BUFFER_SIZE = 729;
	Tryte const BANK_START{ "MMM" };
	int64_t static constexpr BANK_SIZE = 6561;
	Tryte const BANK_END = BANK_START + BANK_SIZE;
	MemoryBlock con_bank{BANK_START, BANK_END};

	// useful memory addresses
	Tryte const COUT_BUFFER_START = BANK_START; // $MMM
	Tryte const CIN_BUFFER_START = COUT_BUFFER_START + BUFFER_SIZE; // $LMM
	// CPU needs to tell the console how many Trytes to print
	// this is stored at this address - reset to 0 after a flush
	Tryte const COUT_BUFFER_LEN = BANK_END - 4; // $Emj
	// CPU can tell how many Trytes are stored in input buffer here
	// overwritten when another read from stdin happens
	Tryte const CIN_BUFFER_LEN = BANK_END - 3; // $Emk
	Tryte const CONSOLE_DISPLAY_MODE = BANK_END - 2; // $Eml
	Tryte const CONSOLE_STATE_ADDR = BANK_END - 1; // $Emm

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