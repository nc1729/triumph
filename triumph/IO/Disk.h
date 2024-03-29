#pragma once

#include <string>
#include <fstream>
#include <array>

#include "common/constants.h"
#include "common/Tryte.h"
#include "Memory/Bank.h"

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
	private Tryte metadata
	*/
	std::array<Tryte, 27> disk_name_trytes;
	Tryte disk_size_tryte;
	Tryte rw_permissions;
	Tryte tilemap_addr;
	Tryte bootcode_addr;
	Tryte page_number;

	Tryte disk_status;
	static int8_t const READWRITE;
	static int8_t const READONLY;
	static int8_t const WRITEONLY;

public:
	std::string const disk_path;
	// which disk is this - first disk is disk 1, etc (same as disk's bank number)
	size_t number;
	// number of pages on disk - between 1 and 19682
	size_t disk_size_pages;
	// check boot status of disk
	bool is_bootable = false;

	// buffer accessible by CPU
	Bank buffer;
	Disk() = delete;
	Disk(size_t const disk_number, std::string const& disk_path);

	

	/*
	useful memory addresses for disk management
	*/
	// buffer start
	static int64_t const DISK_PAGE_START = -9841; // $MMM
	// buffer end
	static int64_t const DISK_PAGE_END = -9841 + 729; // $LMM
	// bank end
	static int64_t constexpr DISK_BANK_END = -9841 + 6561; // $DMM
	// palette location
	static int64_t constexpr DISK_PALETTE_ADDR = -9841 + BootDisk::PALETTE_ARRAY_ADDR; // $ML0

	// disk cache - metadata is cached at the bottom of the disk's memory bank
	// regenerated from Disk object when the disk is read/written
	// cache start
	static int64_t constexpr CACHE_START = DISK_BANK_END - 54; // $ElM
	// disk name - 27 Trytes reserved for this
	static int64_t constexpr CACHE_NAME_ADDR = CACHE_START + BootDisk::NAME_ADDR; // $ElD
	// disk size
	static int64_t constexpr CACHE_SIZE_ADDR = CACHE_START + BootDisk::SIZE_ADDR; // $EmD
	// disk rw permissions
	static int64_t constexpr CACHE_RW_ADDR = CACHE_START + BootDisk::RW_ADDR; // $EmC
	// tilemap pointer
	static int64_t constexpr CACHE_TILEMAP_ADDR = CACHE_START + BootDisk::TILEMAP_PAGE_ADDR; // $EmB
	// bootcode pointer
	static int64_t constexpr CACHE_BCODE_ADDR = CACHE_START + BootDisk::BOOTCODE_PTR_ADDR; // $EmA	
	// which page is open
	static int64_t constexpr CACHE_PAGE_ADDR = DISK_BANK_END - 2; // $Eml
	// live state of the disk - read/write status
	static int64_t constexpr DISK_STATE_ADDR = DISK_BANK_END - 1; // $Emm

	/*
	disk status trits - memory at $Emm is how CPU and disk communicate
	*/
	// disk status flag
	// + : disk is ready for reading/writing
	// 0 : disk is busy
	// - : disk error (see error code - high three trits)
	static size_t const STATUS_FLAG = 0;
	// read request flag
	// + : read requested - disk will begin copying page to buffer when free
	// 0 : do nothing
	// - : do nothing
	static size_t const READ_REQUEST_FLAG = 1;
	// write request flag
	// + : write requested - disk will begin copying page to buffer when free
	// 0 : do nothing
	// - : disk write error (see error code - high three trits)
	static size_t const WRITE_REQUEST_FLAG = 2;
	

	/*
	disk operations
	*/
	// read from the given page (729 Trytes) to the disk_buffer at $0MM-$0mm
	void read_from_page(int64_t const page_number);
	// write to the given page on disk
	void write_to_page(int64_t const page_number);
	// verify disk header
	bool header_is_valid();
	// after read/write, refresh buffer's metadata using internal Disk state - in case of accidental overwriting
	void refresh_metadata();
};