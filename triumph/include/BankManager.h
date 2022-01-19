#pragma once

#include <vector>
#include <cstdint>
#include <map>

#include "Bank.h"

class BankManager
{
private:
	std::vector<Bank*> _bank_ptrs;
	std::map<int64_t, size_t> _label_map;
public:
	BankManager(std::vector<Bank*> const& bank_ptrs);

	Tryte& operator()(int64_t const label, int64_t const addr);
	Tryte const& operator()(int64_t const label, int64_t addr) const;
};