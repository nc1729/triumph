#pragma once

#include <unordered_map>

#include "common/Tryte.h"

class MemoryBlock
{
public:
	MemoryBlock(Tryte const& start = Tryte("MMM"), Tryte const& end = Tryte("MMM"));
	Tryte& operator[](Tryte const& addr);
	Tryte const& operator[](Tryte const& addr) const;

	Tryte const& get_start_addr() { return start_; }
	Tryte const& get_end_addr() { return end_; }

	// get/set index of currently loaded bank
	Tryte& bank() { return current_; }
	// add a bank to the list of loaded banks
	void add_bank(MemoryBlock* bank_ptr, Tryte const& index);
	// remove a bank from list of loaded banks, using its index
	void remove_bank(Tryte const& index);

private:
	// storage in this memory block
	std::unordered_map<Tryte, Tryte> local_;

	// start address of block
	Tryte const start_;

	// end address of block
	Tryte const end_;

	// array of pointers to loaded banks
	std::unordered_map<Tryte, MemoryBlock*> banks_;
	// current bank index
	Tryte current_;
};