#include <iostream>
#include <string>
#include <sstream>

#include "Tryte.h"
#include "Memory.h"
#include "Bank.h"

int main()
{
    Bank bank(0);
    std::vector<Bank*> banks = { &bank };
    Memory memory(1, banks);

    std::cout << memory[-9841] << '\n';
    std::cout << memory[Tryte("MMM")] << '\n';
    std::cout << memory.pc();

    return 0;
}