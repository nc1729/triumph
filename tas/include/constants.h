#pragma once

#include <vector>
#include <string>
#include <map>

namespace constants
{
    std::vector<char> const special_chars =
    {
        '.', '\n', ';', ':', '$', '#', '!', '"'
    };

    std::vector<std::string> const keywords =
    {
        ".func", ".data", ".alias", ".end"
    };

    std::vector<std::string> const regs =
    {
        "A", "B", "C", "D", "E", "F", "G", "H", "I"
    };

    std::vector<std::string> const instructions = 
    {
        "LD", "SV",  "SET", "SWAP", "PUSH", "POP", "IN",  "OUT", "CMP", "JP",
        "JPZ", "JPNZ", "JPP", "JPN",  "AND",  "OR",  "NOT", "ADD", "ADC", "INC",
        "DEC", "SH", "NOP", "BANK", "PEEK", "TJP"
    };

    std::map<std::string, char> const instr_ids =
    {
        {"LD", 'a'}, {"SV", 'A'}, {"SET", 'b'}, {"SWAP", 'B'}, {"PUSH", 'e'},
        {"POP", 'e'}, {"PEEK", 'C'}, {"IN", 'i'}, {"OUT", 'I'}, {"CMP", 'c'},
        {"BANK", 'd'}, {"JP", 'j'}, {"JPZ", 'k'}, {"JPNZ", 'K'},
        {"JPP", 'l'}, {"JPN", 'L'}, {"TJP", 'J'}, {"AND", 'f'}, {"OR", 'F'},
        {"NOT", 'M'}, {"ADD", 'g'}, {"ADC", 'G'}, {"INC", 'h'}, {"DEC", 'H'},
        {"SH", 'm'}, {"NOP", '0'}
    };
}