#include <string>
#include <fstream>
#include <stdexcept>
#include <set>

#include "Tryte.h"
#include "Disk.h"
Disk::Disk(int64_t const disk_number, std::string const& disk_path) : 
	disk_path{disk_path}, buffer{disk_number}
{
	// calculate disk_size
	file_handle.open(disk_path);
	if (!file_handle)
	{
		std::string err_msg = "Couldn't open disk at " + disk_path;
		throw std::runtime_error(err_msg);
	}
	file_handle.seekg(0, std::ios::end);
	disk_size = file_handle.tellg();

	file_handle.close();

	if (!is_valid())
	{
		std::string err_msg = disk_path + " is not a valid TRIUMPH disk file.";
		throw std::runtime_error(err_msg);
	}
	

}

bool Disk::is_valid()
{
	// check that disk contains a positive integer number of pages
	if (disk_size % PAGE_SIZE != 0 || disk_size == 0)
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
	return true;
}

void Disk::read_from_page(size_t const page_number)
{
	file_handle.open(disk_path);
	file_handle.seekg(PAGE_SIZE * page_number, std::ios::beg);
	for (size_t i = 0; i < 729; i++)
	{
		file_handle >> buffer[i];
	}
	file_handle.close();
}

void Disk::write_to_page(size_t const page_number)
{
	file_handle.open(disk_path);
	file_handle.seekg(PAGE_SIZE * page_number, std::ios::beg);
	for (size_t i = 0; i < 729; i++)
	{
		file_handle << buffer[i];
	}
	file_handle.close();
}