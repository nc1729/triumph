#include <vector>
#include <map>

#include "BankManager.h"
#include "Bank.h"
#include "Tryte.h"

BankManager::BankManager(std::vector<Bank*> const& bank_ptrs) : _bank_ptrs{bank_ptrs}
{
    // construct label map
    size_t index = 0;
    for (auto const& bp : _bank_ptrs)
    {
        _label_map[bp->number] = index;
        index++;
    }
}

Tryte& BankManager::operator()(int64_t const label, int64_t const addr)
{
    Bank* bank = _bank_ptrs[_label_map.at(label)];
    // map addr from "Tryte" memory space [-9841, -3280) to [0, 6561)
    return (*bank)[static_cast<size_t>(addr + 9841)];
}

Tryte const& BankManager::operator()(int64_t const label, int64_t const addr) const
{
    Bank* bank = _bank_ptrs[_label_map.at(label)];
    // map addr from "Tryte" memory space [-9841, -3280) to [0, 6561)
    return (*bank)[static_cast<size_t>(addr + 9841)];
}