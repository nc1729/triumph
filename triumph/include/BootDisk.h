#pragma once

/*
locations of important data on boot disk header
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
namespace BootDisk
{
	size_t constexpr SIGNATURE_ADDR = 0;
	size_t constexpr SIGNATURE_SIZE = 9;
	size_t constexpr NAME_ADDR = 9;
	size_t constexpr SIZE_ADDR = 36;
	size_t constexpr RW_ADDR = 37;
	size_t constexpr TILEMAP_PAGE_ADDR = 38;
	size_t constexpr BOOTCODE_PTR_ADDR = 39;
	size_t constexpr PALETTE_ARRAY_ADDR = 40;
}