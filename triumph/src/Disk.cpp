#include <string>
#include <fstream>
#include <stdexcept>
#include <array>
#include <set>

#include "Tryte.h"
#include "Disk.h"

int8_t const Disk::READWRITE = -1;
int8_t const Disk::READONLY = 0;
int8_t const Disk::WRITEONLY = -1;

Disk::Disk(size_t const disk_number, std::string const& disk_path) : 
	disk_path{ disk_path }, number{ disk_number }, buffer{ static_cast<int64_t>(disk_number) }
{
	// calculate disk_size
	file_handle.open(disk_path);
	if (!file_handle)
	{
		std::string err_msg = "Couldn't open disk at " + disk_path;
		throw std::runtime_error(err_msg);
	}
	file_handle.seekg(0, std::ios::end);
	disk_size_chars = file_handle.tellg();
	disk_size_pages = disk_size_chars / PAGE_SIZE;

	file_handle.close();

	if (!is_valid())
	{
		std::string err_msg = disk_path + " is not a valid TRIUMPH disk file.";
		throw std::runtime_error(err_msg);
	}

	// load control Trytes and metadata
	// set disk name
	for (size_t i = 0; i < 27; i++)
	{
		disk_name_trytes[i] = buffer[BootDisk::NAME_ADDR + i];
	}

	// set disk size
	disk_size_tryte = buffer[BootDisk::SIZE_ADDR];

	// set read/write permissions
	rw_permissions = buffer[BootDisk::RW_ADDR];

	// set tilemap page addr
	tilemap_addr = buffer[BootDisk::TILEMAP_PAGE_ADDR];

	// set bootcode ptr
	bootcode_addr = buffer[BootDisk::BOOTCODE_PTR_ADDR];

	// set page number to page 0 (as unsigned Tryte -9841)
	page_number = -9841;
	buffer[CACHE_PAGE_ADDR + 9841] = page_number;

	// set disk status to 1 - disk is ready
	disk_status = 1;
	buffer[DISK_STATE_ADDR + 9841] = disk_status;

	// set bootable flag for computer to attempt boot!
	// disk is bootable if it has a valid header and is big enough to contain a 6561-Tryte tilemap
	if (is_bootdisk())
	{
		is_bootable = true;
	}

}

bool Disk::is_valid()
{
	// check that disk contains a positive integer number of pages
	if (disk_size_chars % PAGE_SIZE != 0 || disk_size_pages == 0)
	{
		return false;
	}

	char c;
	std::set<char> septavingt_chars{ 'M', 'L', 'K', 'J', 'I', 'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A', '0',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm' };
	file_handle.open(disk_path);
	while (file_handle >> c)
	{
		// this may be inefficient
		if (septavingt_chars.find(c) == septavingt_chars.end())
		{
			// found an invalid character
			return false;
		}
	}
	file_handle.close();

	// finally check if header is valid
	return header_is_valid();
}

bool Disk::is_bootdisk()
{
	// header is valid and we know we have at least one page (and we've read it already!)
	// to be bootable, we need a tilemap address (if running in graphics mode)

	// convert signed Tryte to 'unsigned' Tryte
	int64_t tilemap_page_start = buffer[BootDisk::TILEMAP_PAGE_ADDR];
	size_t tilemap_page_start_unsigned = 0;
	if (tilemap_page_start < 0)
	{
		tilemap_page_start_unsigned = tilemap_page_start + 19683;
	}
	else
	{
		tilemap_page_start_unsigned = tilemap_page_start;
	}

	if (tilemap_page_start_unsigned + 9 > disk_size_pages)
	{
		// no room for valid tilemap on disk - disk is not bootable
		return false;
	}
	return true;
}

void Disk::read_from_page(int64_t const page_number)
{
	// set disk status flag to busy
	buffer[DISK_STATE_ADDR + 9841][Disk::STATUS_FLAG] = 0;

	// calculate 'unsigned' page number
	size_t unsigned_page_number = 0;
	if (page_number < 0)
	{
		unsigned_page_number = page_number + 19683;
	}
	else
	{
		unsigned_page_number = page_number;
	}

	// check that we're accessing a valid page
	if (unsigned_page_number >= disk_size_pages)
	{
		// set disk error trit
		buffer[DISK_STATE_ADDR + 9841][STATUS_FLAG] = -1;
		// do nothing else to buffer
		return;
	}

	// check that the hardwired permissions are being respected
	if (rw_permissions[0] == WRITEONLY)
	{
		// set disk error trit
		buffer[DISK_STATE_ADDR + 9841][STATUS_FLAG] = -1;
		// do nothing else to buffer
		return;
	}

	// open disk file and copy to buffer
	file_handle.open(disk_path);
	file_handle.seekg(PAGE_SIZE * unsigned_page_number, std::ios::beg);
	for (size_t i = 0; i < 729; i++)
	{
		file_handle >> buffer[i];
	}
	file_handle.close();

	// set disk status flag to free
	buffer[DISK_STATE_ADDR + 9841][Disk::STATUS_FLAG] = 1;

	refresh_metadata();
}

void Disk::write_to_page(int64_t const page_number)
{
	// set disk status flag to busy
	buffer[DISK_STATE_ADDR + 9841][Disk::STATUS_FLAG] = 0;

	// calculate 'unsigned' page number
	size_t unsigned_page_number = 0;
	if (page_number < 0)
	{
		unsigned_page_number = page_number + 19683;
	}
	else
	{
		unsigned_page_number = page_number;
	}

	// check that we're accessing a valid page
	if (unsigned_page_number >= disk_size_pages)
	{
		// set disk error trit
		buffer[DISK_STATE_ADDR + 9841][STATUS_FLAG] = -1;
		// do nothing else to buffer
		return;
	}

	// check that the hardwired permissions are being respected
	if (rw_permissions[0] == READONLY)
	{
		// set disk error trit
		buffer[DISK_STATE_ADDR + 9841][STATUS_FLAG] = -1;
		// do nothing else to buffer
		return;
	}

	// open disk file and write from buffer
	file_handle.open(disk_path);
	file_handle.seekg(PAGE_SIZE * unsigned_page_number, std::ios::beg);
	for (size_t i = 0; i < 729; i++)
	{
		file_handle << buffer[i];
	}
	file_handle.close();

	// set disk status flag to free
	buffer[DISK_STATE_ADDR + 9841][Disk::STATUS_FLAG] = 1;

	refresh_metadata();
}

bool Disk::header_is_valid()
{
	// read 'boot sector', first page
	this->read_from_page(0);
	// check disk header (TRIUMPH in 'wide' encoding, padded with \0s - one Tryte per char, using ASCII for each char)
	// T R I U M P H -> 84 82 73 85 77 80 72 00 00 -> 0cc 0ca 0cH 0cd 0cD 0cA 0cI 000 000
	std::array<Tryte, BootDisk::SIGNATURE_SIZE> boot_header{ buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8]};
	std::array<Tryte, BootDisk::SIGNATURE_SIZE> header{ Tryte("0cc"), Tryte("0ca"), Tryte("0cH"), Tryte("0cd"), Tryte("0cD"), Tryte("0cA"), Tryte("0cI"), Tryte("000"), Tryte("000")};
	return boot_header == header;
}

void Disk::refresh_metadata()
{
	// refresh disk name
	for (size_t i = 0; i < 27; i++)
	{
		buffer[CACHE_NAME_ADDR + i + 9841] = disk_name_trytes[i];
	}

	// refresh disk size
	buffer[CACHE_SIZE_ADDR + 9841] = disk_size_tryte;

	// refresh read/write permissions
	buffer[CACHE_RW_ADDR + 9841] = rw_permissions;

	// refresh tilemap page
	buffer[CACHE_TILEMAP_ADDR + 9841] = tilemap_addr;

	// refresh bootcode ptr
	buffer[CACHE_BCODE_ADDR + 9841] = bootcode_addr;

	// refresh page number
	buffer[CACHE_PAGE_ADDR + 9841] = page_number;
}