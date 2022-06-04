#pragma once
#include <iostream>

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
public:
	Console(std::ostream& out, std::istream& in) : out{ out }, in{ in } {};
	DisplayMode mode = DisplayMode::RAW;
	int64_t static constexpr CONSOLE_BANK = -4;
	Bank buffer{ CONSOLE_BANK };

	// useful memory addresses
	static int64_t const COUT_BUFFER_START = -9841; // $MMM
	static int64_t const CIN_BUFFER_START = -9841 + 729; // $LMM
	static int64_t const CONSOLE_BANK_END = -9841 + 6561; // $DMM
	// CPU needs to tell the console how many Trytes to print
	// this is stored at this address - reset to 0 after a flush
	static int64_t const COUT_BUFFER_LEN = CONSOLE_BANK_END - 4; // $Emj
	// CPU can tell how many Trytes are stored in input buffer here
	// overwritten when another read from stdin happens
	static int64_t const CIN_BUFFER_LEN = CONSOLE_BANK_END - 3; // $Emk
	static int64_t const CONSOLE_DISPLAY_MODE = CONSOLE_BANK_END - 2; // $Eml
	static int64_t const CONSOLE_STATE_ADDR = CONSOLE_BANK_END - 1; // $Emm

	/*
	console status trits - memory at $Emm 
	*/
	// console status flag
	// + : console is ready for input/output
	// 0 : console is busy (printing or waiting for input)
	// - : console error
	static size_t const STATUS_FLAG = 0;
	// out request flag
	// + : flush buffer to out stream when ready
	// 0 : do nothing
	// - : do nothing
	static size_t const OUT_REQUEST_FLAG = 1;
	// in request flag
	// + : fetch input from in stream
	// 0 : do nothing
	// - : do nothing
	static size_t const IN_REQUEST_FLAG = 2;

	/*
	console operations
	*/
	void flush_to_out();
	void read_from_in();
};