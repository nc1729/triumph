#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "constants.h"
#include "util.hpp"
#include "CPU.h"

void CPU::debug()
{
    std::cout << "--- TRIUMPH Debugger ---\n";
    std::cout << "Press h for help, r to exit debugger (run program) or q to quit.\n";
    std::string input;
    std::string word;
    std::vector<std::string> words;
    // get first input
    std::cout << ">>> ";
    std::getline(std::cin, input);
    // loop until debugger is quit
    while (debug_mode_)
    {
        // break line input into words
        std::stringstream input_stream(input);
        while (input_stream >> word)
        {
            words.push_back(word);
        }

        if (words.size() == 0)
        {
            // user just pressed ENTER
            continue;
        }

        std::string& command = words[0];
        // parse command
        if (command == "q" || command == "quit")
        {
            // exit debug mode and switch off CPU
            debug_mode_ = false;
            on_ = false;
        }
        else if (command == "r" || command == "run")
        {
            // exit debug mode and continue normal CPU operation
            debug_mode_ = false;
        }
        else if (command == "n" || command == "next")
        {
            // run next instruction
            fetch();
            decode_and_execute();
            dump();
        }
        else if (command == "pc")
        {
            std::cout << "Program counter: " << pc_ << '\n';
        }
        else if (command == "stack")
        {
            std::cout << "Stack pointer: " << sp_ <<'\n';
            if (sp_ > Memory::STACK_BOTTOM)
            {
                std::cout << "Stack has underflowed\n";
                continue;
            }
            else if (sp_ < Memory::STACK_TOP)
            {
                std::cout << "Stack has overflowed\n";
                continue;
            }
            // print contents of stack
            Tryte stack_start = sp_;
            while (stack_start != Memory::STACK_BOTTOM)
            {
                std::cout << stack_start << ": " << memory_[stack_start] << '\n';
                stack_start += 1;
            }
            std::cout << Memory::STACK_BOTTOM << ": " << memory_[stack_start] << "\n\n";
        }
        else if (command == "bank")
        {
            std::cout << "Memory bank: " << memory_[Memory::BANK] << '\n';
        }
        else if (command == "c" || command == "current")
        {
            std::cout << "Current instruction: " << instr_;
        }
        else if (command == "s" || command == "set")
        {
            debug_set_command(words);
        }
        else if (command == "p" || command == "print")
        {
            debug_print(words);
        }
        else if (command == "d" || command == "dump")
        {
            debug_dump_memory(words);
        }
        else if (command == "h" || command == "help")
        {
            debug_show_help();
        }
        else
        {
            std::cout << "Unrecognised command.\n";
        }

        words.clear();

        if (debug_mode_)
        {
            // get next input
            std::cout << ">>> ";
            std::getline(std::cin, input);
        }
    }
}

void CPU::debug_set_command(std::vector<std::string> const& words)
{
    if (words.size() != 3)
    {
        std::cout << "Usage: s(et) [REG/$ADDR/BANK] [VAL]\n";
        return;
    }

    std::string reg;
    if (util::is_in(constants::regs, words[1]))
    {
        // first argument was a register
        // first make it lower case
        reg = util::to_lower(words[1]);
        // 'a' = 97, 'i' = 105; this converts 'a' -> 1, 'b' -> 2
        // which is their index in the regs_ array
        size_t reg_index = static_cast<size_t>(reg[0]) - 96;

        // now handle the second argument
        if (util::string_is_septavingt(words[2]))
        {
            set(regs_[reg_index], Tryte(words[2].substr(2)));
            return;
        }
        else if (util::string_is_tryte(words[2]))
        {
            set(regs_[reg_index], Tryte(words[2]));
            return;
        }
        else if (util::string_is_int(words[2]))
        {
            set(regs_[reg_index], Tryte(std::stoi(words[2])));
            return;
        }

    }
    else if (util::string_is_addr(words[1]))
    {
        // first argument was an address
        Tryte addr = Tryte(words[1].substr(1));
        // now handle the second argument
        if (util::string_is_septavingt(words[2]))
        {
            set(memory_[addr], Tryte(words[2].substr(2)));
            return;
        }
        else if (util::string_is_tryte(words[2]))
        {
            set(memory_[addr], Tryte(words[2]));
            return;
        }
        else if (util::string_is_int(words[2]))
        {
            set(memory_[addr], Tryte(std::stoi(words[2])));
            return;
        }
    }
    else if (words[1] == "bank" || words[1] == "BANK")
    {
        // handle second argument
        if (util::string_is_septavingt(words[2]))
        {
            memory_[Memory::BANK] = Tryte(words[2].substr(2));
            return;
        }
        else if (util::string_is_tryte(words[2]))
        {
            memory_[Memory::BANK] = Tryte(words[2]);
            return;
        }
        else if (util::string_is_int(words[2]))
        {
            memory_[Memory::BANK] = Tryte(std::stoi(words[2]));
            return;
        }

    }

    // if we got here, arguments were invalid
    std::cout << "Usage: s(et) [REG/$ADDR/BANK] [VAL]\n";
    return;

}

void CPU::debug_print(std::vector<std::string> const& words)
{
    if (words.size() == 1)
    {
        std::cout << "Usage: p(rint) [REG/$ADDR] [REG/$ADDR] ...\n";
        return;
    }

    // iterate through arguments and print them
    for (size_t i = 1; i < words.size(); i++)
    {
        // check if arg is a register or an address
        if (util::is_in(constants::regs, words[i]))
        {
            // argument was a register
            // first make it lower case
            std::string reg = util::to_lower(words[1]);
            // 'a' = 97, 'i' = 105; this converts 'a' -> 1, 'b' -> 2
            // which is their index in the regs_ array
            size_t reg_index = static_cast<size_t>(reg[0]) - 96;

            // now print it
            std::cout << reg << ": " << regs_[reg_index] << '\n';
        }
        else if (util::string_is_addr(words[i]))
        {
            // argument was an address
            Tryte addr = Tryte(words[1].substr(1));
            std::cout << words[i] << ": " << memory_[addr] << '\n';
        }
        else
        {
            std::cout << words[i] << " is not a register or address.\n";
        }
    }
}

void CPU::debug_dump_memory(std::vector<std::string> const& words)
{
    if (words.size() != 3)
    {
        std::cout << "Usage: d(ump) $ADDR1 $ADDR2/VAL\n";
        return;
    }

    Tryte addr1;
    Tryte addr2;

    // check first arg was an address
    if (!(util::string_is_addr(words[1])))
    {
        std::cout << "Usage: d(ump) $ADDR1 $ADDR2\n";
        return;
    }
    addr1 = Tryte(words[1].substr(1));

    // check second arg was address or value
    if (util::string_is_addr(words[2]))
    {
        addr2 = Tryte(words[2].substr(1));
    }
    else if (util::string_is_tryte(words[2]))
    {
        addr2 = addr1 + Tryte(words[2]);
    }
    else if (util::string_is_int(words[2]))
    {
        addr2 = addr1 + Tryte(std::stoi(words[2]));
    }

    while (addr1 != addr2)
    {
        std::cout << addr1 << ": " << memory_[addr1] << '\n';
        addr1 += 1;
    }
}

void CPU::debug_show_help()
{
    std::cout << "Commands:\n";
    std::cout << "bank                            : print current bank\n";
    std::cout << "c(urrent)                       : print current instruction opcode\n";
    std::cout << "d(ump) $ADDR1 $ADDR2            : dump memory between two addresses\n";
    std::cout << "h(elp)                          : print this help\n";
    std::cout << "n(ext)                          : run next instruction\n";
    std::cout << "pc                              : print program counter\n";
    std::cout << "p(rint) REG/ADDR1 REG/ADDR2 ... : print list of registers/memory addresses\n";
    std::cout << "q(uit)                          : shut down TRIUMPH\n";
    std::cout << "r(un)                           : exit debug mode and resume normal CPU operation\n";
    std::cout << "s(et) REG/ADDR/BANK VAL         : set register/memory address to value\n";
    std::cout << "stack                           : print contents of stack\n";
}