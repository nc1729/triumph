#pragma once

#include <vector>
#include <cstdint>
#include <map>

#include "Memory/Bank.h"

class BankManager
{
private:
	std::vector<Bank*> banks_;
	std::map<int64_t, size_t> label_map_;
public:
	BankManager(std::vector<Bank*> const& banks);

	void add_bank(Bank* new_bank_ptr);
	size_t size();

	Tryte& operator()(int64_t const label, int64_t const addr);
	Tryte const& operator()(int64_t const label, int64_t addr) const;
};