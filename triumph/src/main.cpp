#include <iostream>
#include <string>
#include <sstream>

#include "Tryte.h"
#include "Memory.h"
#include "Device.h"
#include "Bank.h"

int main()
{
    /*
    Bank bank0(0);
    Bank bank1(1);
    std::vector<Bank*> banks = { &bank0, &bank1 };
    Memory memory(banks.size(), banks);

    memory[-9841] = -54;
    std::cout << memory[-9841] << '\n';
    // switch bank
    memory.bank() = 1;
    memory[-9841] = 27;
    std::cout << memory[-9841] << '\n';
    memory.bank() = 0;
    std::cout << memory[-9841] << '\n';
    std::cout << memory.pc();
    */
    
    Tryte t;
    Device d(&std::cin, &std::cout);
    d.num_mode();
    while (true)
    {
        try
        {
            d >> t;
            d << t;
            std::cout << '\n';
        }
        catch(const std::out_of_range& e)
        {
            std::cerr << "Invalid Tryte expression" << '\n';
        }
    }
    
    

    return 0;
}