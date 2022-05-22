#pragma once

#include <string>
#include <fstream>
#include "Bank.h"

class Disk
{
private:
	std::fstream file_handle;
	size_t disk_size_chars;
	// 729 Trytes in a page - 2187 chars in a page
	size_t static const PAGE_SIZE = 2187;
	// validate disk - check all chars are septavingtesmal and size of disk is a positive integer number of pages
	bool is_valid();
	// check if disk is bootable
	bool is_bootdisk();

	/*
	header constants (relative to buffer)
	disk header layout is:
		9 Trytes  - disk signature
		27 Trytes - disk name
		1 Tryte   - disk size (in pages); unsigned tryte
		1 Tryte   - disk read/write permissions and other flags
		1 Tryte	  - tilemap page start (needs to be 9 pages in front of this); unsigned Tryte
		1 Tryte	  - boot code start ptr (usually M00)
		81 Trytes - 3 Tryte palette (3 9-trit colours) in order from -13 to 13
		243 Trytes - if boot code starts at M00, free space
		365 Trytes - boot code; load tilemap, zero registers, etc...
	*/
	// disk signature location
	size_t static constexpr BUFFER_SIG_ADDR = 0; // "TRIUMPH  "
	// disk signature size
	size_t static constexpr BUFFER_SIG_SIZE = 9;
	// disk name location
	size_t static constexpr BUFFER_NAME_ADDR = 9;
	// disk size location
	size_t static constexpr BUFFER_SIZE_ADDR = 36;
	// disk r/w permissions location (and default state)
	size_t static constexpr BUFFER_STATE_ADDR = 37;
	// tilemap page start location
	size_t static constexpr BUFFER_TILEMAP_ADDR = 38;
	// boot code ptr location
	size_t static constexpr BUFFER_BOOT_ADDR = 39;
	// palette location
	size_t static constexpr BUFFER_PALETTE_ADDR = 40;

public:
	std::string const disk_path;
	// which disk is this - first disk is disk 1, etc (same as disk's bank number)
	size_t number;
	// number of pages on disk - between 1 and 19682
	size_t disk_size_pages;
	// check boot status of disk
	bool is_bootable = false;
	enum class Status
	{
		READWRITE, READONLY, WRITEONLY
	};
	Status status;
	// buffer accessible by CPU
	Bank buffer;
	Disk() = delete;
	Disk(size_t const disk_number, std::string const& disk_path);

	

	/*
	control Tryte addresses - metadata about the disk is stored in disk_buffer at the following addresses
	these are read from disk when it is loaded
	*/
	// buffer start
	static int64_t const DISK_BUFF_START = -9841; // $MMM
	// buffer end
	static int64_t const DISK_BUFF_END = -9112; // $LMM
	// palette location
	size_t static constexpr DISK_PALETTE_ADDR = -9801;
	// disk name - 27 Trytes reserved for this
	static int64_t const DISK_NAME_ADDR = -3335; // $Ekm
	// bootcode pointer
	static int64_t const DISK_BCODE_ADDR = -3283; // $Emi
	// tilemap pointer
	static int64_t const DISK_TILEMAP_ADDR = -3284; // $Emj
	// disk size
	static int64_t const DISK_SIZE_ADDR = -3283; // $Emk
	// which page is open
	static int64_t const DISK_PAGE_ADDR = -3282; // $Eml
	// state of the disk - read/write status
	static int64_t const DISK_STATE_ADDR = -3281; // $Emm

	/*
	disk status trits - memory at $Emm is how CPU and disk communicate
	*/
	// GS flag
	// + : disk can be read/written to
	// 0 : disk is read only
	// - : disk is write only
	static size_t const PERMISSIONS_FLAG = 0; // lowest trit
	// read request flag
	// + : read requested - disk will begin copying page to buffer when free
	// 0 : do nothing
	// - : do nothing
	static size_t const READ_REQUEST_FLAG = 1;
	// read status flag
	// + : disk is ready for reading
	// 0 : disk is busy
	// - : disk read error (see error code - high three trits)
	static size_t const WRITE_REQUEST_FLAG = 2;
	// write status flag
	// + : disk is ready for reading/writing
	// 0 : disk is busy
	// - : disk error (see error code - high three trits)
	static size_t const STATUS_FLAG = 3;

	/*
	disk operations
	*/
	// read from the given page (729 Trytes) to the disk_buffer at $0MM-$0mm
	void read_from_page(size_t const page_number);
	// write to the given page on disk
	void write_to_page(size_t const page_number);
	// verify disk header
	bool header_is_valid();
	// after read/write, refresh buffer's metadata using internal Disk state
	void refresh_metadata();
};