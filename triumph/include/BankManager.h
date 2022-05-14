#pragma once

#include <vector>
#include <cstdint>
#include <map>

#include "Bank.h"

class BankManager
{
private:
	std::vector<Bank> banks_;
	std::map<int64_t, size_t> label_map_;
public:
	BankManager(std::vector<Bank> const& banks);

	Tryte& operator()(int64_t const label, int64_t const addr);
	Tryte const& operator()(int64_t const label, int64_t addr) const;
};