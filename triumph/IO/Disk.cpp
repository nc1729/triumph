#include <string>
#include <fstream>
#include <stdexcept>
#include <array>
#include <set>

#include "common/Tryte.h"
#include "IO/Disk.h"

Tryte const Disk::PAGE_SIZE = 729;
Tryte const Disk::BANK_SIZE{ 729 + 27 }; // page + 27 status Trytes

Tryte const Disk::PAGE_START{ "MMM" };
Tryte const Disk::PAGE_END = Disk::PAGE_START + Disk::PAGE_SIZE; // $LMM
Tryte const Disk::BANK_END = Disk::PAGE_START + Disk::BANK_SIZE; // $LLM

// after reading a page from disk, metadata about the new page is stored for later use
// the size of the disk (in pages)
Tryte const Disk::SIZE{ Disk::BANK_END - 3 }; // $LMk
// the current page
Tryte const Disk::PAGE{ Disk::BANK_END - 2 }; // $LMl
// various state flags for requesting read/write
Tryte const Disk::STATE{ Disk::BANK_END - 1 }; // $LMm

Disk::Disk(std::string const& disk_path) : 
	disk_path{ disk_path }
{
	// set up buffer
	disk_bank.add_bank(&buffer, 1);

	// calculate disk_size
	file_handle.open(disk_path);
	if (!file_handle)
	{
		std::string err_msg = "Couldn't open disk at " + disk_path;
		throw std::runtime_error(err_msg);
	}
	file_handle.seekg(0, std::ios::end);
	disk_size_chars = file_handle.tellg();
	disk_size_pages = disk_size_chars / CHAR_PAGE_SIZE;

	file_handle.close();

	/*
	if (!is_valid())
	{
		std::string err_msg = disk_path + " is not a valid TRIUMPH disk file.";
		throw std::runtime_error(err_msg);
	}
	*/

	// load control Trytes and metadata
	// set disk name
	
	/*
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

	// set default page number to page 0
	buffer[CACHE_PAGE_ADDR + 9841] = 0;

	// set disk status to 1 - disk is ready
	disk_status = 1;
	buffer[DISK_STATE_ADDR + 9841] = disk_status;

	// set bootable flag for computer to attempt boot!
	// disk is bootable if it has a valid header and is big enough to contain a 6561-Tryte tilemap
	if (is_bootdisk())
	{
		is_bootable = true;
	}
	*/
	disk_bank[SIZE] = disk_size_pages;
	disk_bank[PAGE] = 0;
	disk_bank[STATE] = 0;
	disk_bank[STATE][STATUS_FLAG] = READY;
	disk_bank[STATE][RW_STATUS_FLAG] = READWRITE; // read disk header for this?

}

/*
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
*/

/*
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
*/

void Disk::read_from_page(Tryte const page_number)
{
	// set disk status flag to busy
	disk_bank[STATE][STATUS_FLAG] = 0;

	// calculate 'unsigned' page number
	size_t unsigned_page_number = 0;
	if (page_number < 0)
	{
		unsigned_page_number = page_number.get_int() + 19683;
	}
	else
	{
		unsigned_page_number = page_number.get_int();
	}

	// check that we're accessing a valid page
	if (unsigned_page_number >= disk_size_pages)
	{
		// set disk error trit
		disk_bank[STATE][STATUS_FLAG] = ERROR;
		// do nothing else to buffer
		return;
	}

	// check that the hardwired permissions are being respected
	if (disk_bank[STATE][RW_STATUS_FLAG] == WRITEONLY)
	{
		// set disk error trit
		disk_bank[STATE][STATUS_FLAG] = ERROR;
		// do nothing else to buffer
		return;
	}

	// open disk file and copy to buffer
	// always read to inactive buffer to prevent data race
	MemoryBlock* dest;
	if (disk_bank.bank() == 0)
	{
		dest = &buffer;
	}
	else
	{
		dest = &disk_bank;
	}
	Tryte start = dest->get_start_addr();
	file_handle.open(disk_path);
	file_handle.seekg(PAGE_SIZE.get_int() * unsigned_page_number, std::ios::beg);
	for (Tryte i = 0; i < PAGE_SIZE; ++i)
	{
		file_handle >> (*dest)[start + i];
	}
	file_handle.close();

	// set disk status flag to free
	disk_bank[STATE][STATUS_FLAG] = READY;
	// swap buffers (CPU only ever accesses other buffer)
	disk_bank.bank() = 1 - disk_bank.bank();
	// refresh metadata
	disk_bank[SIZE] = disk_size_tryte;
	disk_bank[PAGE] = page_number;

}

void Disk::write_to_page(Tryte const page_number)
{
	// set disk status flag to busy
	disk_bank[STATE][STATUS_FLAG] = 0;

	// calculate 'unsigned' page number
	size_t unsigned_page_number = 0;
	if (page_number < 0)
	{
		unsigned_page_number = page_number.get_int() + 19683;
	}
	else
	{
		unsigned_page_number = page_number.get_int();
	}

	// check that we're accessing a valid page
	if (unsigned_page_number >= disk_size_pages)
	{
		// set disk error trit
		disk_bank[STATE][STATUS_FLAG] = ERROR;
		// do nothing else to buffer
		return;
	}

	// check that the hardwired permissions are being respected
	if (disk_bank[STATE][RW_STATUS_FLAG] == WRITEONLY)
	{
		// set disk error trit
		disk_bank[STATE][STATUS_FLAG] = ERROR;
		// do nothing else to buffer
		return;
	}

	// write contents of active buffer to inactive buffer
	// this copy makes write_to_page invisible to CPU
	MemoryBlock* page_to_write;
	MemoryBlock* copy;
	if (disk_bank.bank() == 0)
	{
		page_to_write = &disk_bank;
		copy = &buffer;
	}
	else
	{
		page_to_write = &buffer;
		copy = &disk_bank;
	}
	Tryte write_start = page_to_write->get_start_addr();
	Tryte copy_start = copy->get_start_addr();
	for (Tryte i = 0; i < PAGE_SIZE; ++i)
	{
		(*copy)[copy_start + i] = (*page_to_write)[write_start + i];
	}
	// swap buffers
	disk_bank.bank() = 1 - disk_bank.bank();

	// write the now inactive buffer to file
	file_handle.open(disk_path);
	file_handle.seekg(PAGE_SIZE.get_int() * unsigned_page_number, std::ios::beg);
	for (Tryte i = 0; i < PAGE_SIZE; ++i)
	{
		file_handle << (*page_to_write)[write_start + i];
	}
	file_handle.close();

	// set disk status flag to free
	disk_bank[STATE][STATUS_FLAG] = 1;
	
	// refresh metadata
	disk_bank[SIZE] = disk_size_tryte;
	disk_bank[PAGE] = page_number;
}

/*
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
}
*/
