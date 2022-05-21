#pragma once

#include <string>
#include <fstream>
#include "Bank.h"

class Disk
{
private:
	std::string disk_path;
	std::fstream file_handle;
	size_t disk_size;
	// 729 Trytes in a page - 2187 chars in a page
	size_t static const PAGE_SIZE = 2187;
	// validate disk - check all chars are septavingtesmal and size of disk is a positive integer number of pages
	bool is_valid();
public:
	Bank disk_buffer;
	Disk() = delete;
	Disk(int64_t const disk_number, std::string const& disk_path);

	/*
	control Tryte addresses - metadata about the disk is stored in disk_buffer at the following addresses
	these are read from disk when it is loaded
	*/
	// buffer start
	size_t static const DISK_BUFF_START = -9841; // $MMM
	// buffer end
	size_t static const DISK_BUFF_END = -9112; // $LMM
	// disk name - 27 Trytes reserved for this (54 chars with compact encoding (six trits per char))
	size_t static const DISK_NAME_ADDR = -3335; // $Ekm
	// disk size
	size_t static const DISK_SIZE_ADDR = -3282; // $Eml
	// state of the disk - read/write status
	size_t static const DISK_STATE_ADDR = -3281; // $Emm

	/*
	disk operations
	*/
	// read from the given page (729 Trytes) to the disk_buffer at $0MM-$0mm
	void read_from_disk(size_t const page_number);
	// write to the given page on disk
	void write_to_disk(size_t const page_number);
};