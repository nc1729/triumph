#include <vector>
#include <map>

#include "common/Tryte.h"
#include "Memory/BankManager.h"
#include "Memory/Bank.h"

BankManager::BankManager(std::vector<Bank*> const& banks) : banks_{banks}
{
    // construct label map
    size_t index = 0;
    for (auto const& bp : banks_)
    {
        label_map_[bp->number] = index;
        index++;
    }
}

void BankManager::add_bank(Bank* new_bank_ptr)
{
    banks_.push_back(new_bank_ptr);
    label_map_[new_bank_ptr->number] = banks_.size() - 1;
}

size_t BankManager::size()
{
    return banks_.size();
}

Tryte& BankManager::operator()(int64_t const label, int64_t const addr)
{
    Bank* bank = banks_[label_map_.at(label)];
    // map addr from "Tryte" memory space [-9841, -3280) to [0, 6561)
    return (*bank)[static_cast<size_t>(addr + 9841)];
}

Tryte const& BankManager::operator()(int64_t const label, int64_t const addr) const
{
    Bank const* bank = banks_[label_map_.at(label)];
    // map addr from "Tryte" memory space [-9841, -3280) to [0, 6561)
    return (*bank)[static_cast<size_t>(addr + 9841)];
}