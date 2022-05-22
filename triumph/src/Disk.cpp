#include <string>
#include <fstream>
#include <stdexcept>
#include <array>
#include <set>

#include "Tryte.h"
#include "Disk.h"
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
	
	if (is_bootdisk())
	{
		// load control Trytes and metadata
		// load disk name
		for (size_t i = 0; i < 27; i++)
		{
			buffer[DISK_NAME_ADDR + 9841 + i] = buffer[BUFFER_NAME_ADDR + i];
		}

		// load boot code start pointer
		buffer[DISK_BCODE_ADDR + 9841] = buffer[BUFFER_BOOT_ADDR];
		
		// load disk tilemap page start
		buffer[DISK_TILEMAP_ADDR + 9841] = buffer[BUFFER_TILEMAP_ADDR];

		// load disk size
		buffer[DISK_SIZE_ADDR + 9841] = buffer[BUFFER_SIZE_ADDR];

		// set open page to 0
		buffer[DISK_PAGE_ADDR + 9841] = 0;

		// load disk read/write permissions
		buffer[DISK_STATE_ADDR + 9841] = buffer[BUFFER_STATE_ADDR];
		// set internal disk read/write permissions - CPU cannot change this
		if (buffer[BUFFER_STATE_ADDR][PERMISSIONS_FLAG] == 1)
		{
			status = Disk::Status::READWRITE;
		}
		else if (buffer[BUFFER_STATE_ADDR][PERMISSIONS_FLAG] == 0)
		{
			status = Disk::Status::READONLY;
		}
		else
		{
			status = Disk::Status::WRITEONLY;
		}

		// set bootable flag for computer to attempt boot!
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
	size_t tilemap_page_start = Tryte::get_int(buffer[38]) + 9841;
	if (tilemap_page_start + 9 > disk_size_pages)
	{
		// no room for valid tilemap on disk - disk is not bootable
		return false;
	}
	return true;
}

void Disk::read_from_page(size_t const page_number)
{
	// check that we're accessing a valid page
	if (page_number >= disk_size_pages)
	{
		// set disk error trit
		buffer[DISK_STATE_ADDR + 9841][STATUS_FLAG] = -1;
		// do nothing else to buffer
		return;
	}
	if (status == Status::WRITEONLY)
	{
		// set disk error trit
		buffer[DISK_STATE_ADDR + 9841][STATUS_FLAG] = -1;
		// do nothing else to buffer
		return;
	}

	// open disk file and copy to buffer
	file_handle.open(disk_path);
	file_handle.seekg(PAGE_SIZE * page_number, std::ios::beg);
	for (size_t i = 0; i < 729; i++)
	{
		file_handle >> buffer[i];
	}
	file_handle.close();
	// recover disk size
	buffer[DISK_SIZE_ADDR + 9841] = static_cast<int64_t>(disk_size_pages) - 9841;
	// set disk status trit to free
	buffer[DISK_STATE_ADDR + 9841][Disk::STATUS_FLAG] = 1;
}

void Disk::write_to_page(size_t const page_number)
{
	if (page_number >= disk_size_pages)
	{
		// set disk error trit
		buffer[DISK_STATE_ADDR + 9841][STATUS_FLAG] = -1;
		// do nothing else to buffer
		return;
	}
	if (status == Status::READONLY)
	{
		// set disk error trit
		buffer[DISK_STATE_ADDR + 9841][STATUS_FLAG] = -1;
		// do nothing else to buffer
		return;
	}
	file_handle.open(disk_path);
	file_handle.seekg(PAGE_SIZE * page_number, std::ios::beg);
	for (size_t i = 0; i < 729; i++)
	{
		file_handle << buffer[i];
	}
	file_handle.close();
	// recover disk size
	buffer[DISK_SIZE_ADDR + 9841] = static_cast<int64_t>(disk_size_pages) - 9841;
	// set disk status trit to free
	buffer[DISK_STATE_ADDR + 9841][Disk::STATUS_FLAG] = 1;
}

bool Disk::header_is_valid()
{
	// read 'boot sector', first page
	this->read_from_page(0);
	// check disk header (TRIUMPH in 'wide' encoding, padded with \0s - one Tryte per char, using ASCII for each char)
	// T R I U M P H -> 84 82 73 85 77 80 72 00 00 -> 0cc 0ca 0cH 0cd 0cD 0cA 0cI 000 000
	std::array<Tryte, BUFFER_SIG_SIZE> boot_header{ buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8]};
	std::array<Tryte, BUFFER_SIG_SIZE> header{ Tryte("0cc"), Tryte("0ca"), Tryte("0cH"), Tryte("0cd"), Tryte("0cD"), Tryte("0cA"), Tryte("0cI"), Tryte("000"), Tryte("000")};
	return boot_header == header;
}