#include <vector>
#include <string>
#include <map>

namespace constants
{
    std::string const septavingt_chars = "MLKJIHGFEDCBA0abcdefghijklm";
    std::string const ternary_chars = "-0+";

    std::vector<char> const org_chars =
    {
        // block start
        '{',
        // block end
        '}',
        // newline - ends a statement
        '\n',
        // statement end - ends statement without newline
        ';',
        // statement continue char - breaks up Tokens in same statement
        ','
    };

    std::vector<std::string> const regs =
    {
        "A", "B", "C", "D", "E", "F", "G", "H", "I"
    };

    std::vector<std::string> const reg_addrs =
    {
        "[A]", "[B]", "[C]", "[D]", "[E]", "[F]", "[G]", "[H]", "[I]"
    };

    std::vector<std::string> const instructions =
    {
        // memory management
        "LOAD",
        "SAVE",
        "BANK",
        "GANK",
        // register management
        "SET",
        "SWAP",
        // stack management
        "PUSH",
        "POP",
        "PEEK",
        // arithmetic ops
        "ADD",
        "ADC",
        "SUB",
        "SBB",
        "SH",
        "INC",
        "DEC",
        "FLIP",
        "ZERO",
        // logic ops
        "CMP",
        "CPZ",
        "AND",
        "OR",
        "STAR",
        // control flow
        "NOP",
        "HALT",
        "JP",
        "JPS",
        "PJP"
    };

    std::vector<std::string> const macros =
    {
        // equivalent to FLIP
        "NOT",
        // 
        "JPZ",
        "JPN",
        "JPP",
        "JPNZ",
        "JPNN",
        "JPNP"
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