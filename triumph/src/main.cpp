#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <stdexcept>

#include "Tryte.h"
#include "Disk.h"
#include "Computer.h"

std::vector<Tryte> read_input(std::string const& filename)
{
    std::vector<Tryte> program;
    std::string schars = "MLKJIHGFEDCBA0abcdefghijklm";
    char c;
    std::string tryte_str;
    std::fstream input_file{filename};
    while (input_file >> c)
    {
        // ignore any non-septavingt chars
        if (schars.find(c) != std::string::npos)
        {
            // c is a valid septavingt char
            tryte_str += c;
            if (tryte_str.size() == 3)
            {
                // found a whole Tryte, add it to program
                program.emplace_back(tryte_str);
                tryte_str.clear();
            }
        }
    }

    return program;
    
}

int main(int argc, char* argv[])
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

    if (argc < 2)
    {
        std::cerr << "Usage: ./triumph [DISK_FILE1] [DISK_FILE2] ...\n";
        return 1;
    }

    size_t number_of_disks = argc - 1;

    std::vector<Disk> disks;
    for (size_t i = 0; i < number_of_disks; i++)
    {
        // the first disk in the argument list will be assigned to bank 1, then bank 2 etc
        try
        {
            disks.push_back(Disk(i + 1, argv[i + 1]));
        }
        catch (std::runtime_error const& e)
        {
            std::cerr << e.what() << '\n';
            return 1;
        }
    }
    
    try
    {
        Computer computer(disks);
        computer.boot();
    }
    catch(std::runtime_error const& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    

    /*
    if (argc != 3)
    {
        std::cerr << "Usage: ./triumph [INPUT_FILENAME] [TILEMAP_FILENAME]\n";
        return 1;
    }
    std::string const input_filename = argv[1];
    std::vector<Tryte> program = read_input(input_filename);
    std::string const tilemap_filename = argv[2];
    */
    
    // MJ0 00b MI0 00a 000 is the program "PORT 2; OUT 1; HALT"
    //computer.run_program({ Tryte("MJ0"), Tryte("00b"), Tryte("MI0"), Tryte("00a"), Tryte("MMM") });

    // maa 00b mla Laa MMM : SET A, 2; INC A; SHOW A; HALT
    //computer.run_program({Tryte("maa"), Tryte("mmm"), Tryte("mla"), Tryte("Laa"), Tryte("MMM")});

    // LAa eaa Laa MMM: TELL A; ADD A, A; SHOW A; HALT;
    //computer.run_program({Tryte("LAa"), Tryte("eaa"), Tryte("Laa"), Tryte("MMM")});

    //std::thread t{&Computer::run_program, &computer, std::ref(program)};

    //computer.test(tilemap_filename);
    //show_window();
    //t.join();
    //std::cout << '\n';
    return 0;
}
