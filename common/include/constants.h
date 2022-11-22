#pragma once

#include <vector>
#include <string>
#include <array>
#include <map>

#include "Tryte.h"

namespace constants
{
    Tryte const CONSOLE_BANK = -1;
    std::map<int64_t, std::array<int8_t, 3>> const int_to_ternary =
    {
        {-13, {-1, -1, -1}},
        {-12, {-1, -1, 0}},
        {-11, {-1, -1, 1}},
        {-10, {-1, 0, -1}},
        {-9, {-1, 0, 0}},
        {-8, {-1, 0, 1}},
        {-7, {-1, 1, -1}},
        {-6, {-1, 1, 0}},
        {-5, {-1, 1, 1}},
        {-4, {0, -1, -1}},
        {-3, {0, -1, 0}},
        {-2, {0, -1, 1}},
        {-1, {0, 0, -1}},
        {0, {0, 0, 0}},
        {1, {0, 0, 1}},
        {2, {0, 1, -1}},
        {3, {0, 1, 0}},
        {4, {0, 1, 1}},
        {5, {1, -1, -1}},
        {6, {1, -1, 0}},
        {7, {1, -1, 1}},
        {8, {1, 0, -1}},
        {9, {1, 0, 0}},
        {10, {1, 0, 1}},
        {11, {1, 1, -1}},
        {12, {1, 1, 0}},
        {13, {1, 1, 1}}
    };

    // cache some constant tryte values for quick recall (SH instruction)
    std::map<int8_t, Tryte> const int_to_tryte =
    {
        {-13, -13}, {-12, -12}, {-11, -11},
        {-10, -10}, {-9, -9}, {-8, -8},
        {-7, -7}, {-6, -6}, {-5, -5},
        {-4, -4}, {-3, -3}, {-2, -2},
        {-1, -1}, {0, 0}, {1, 1},
        {2, 2}, {3, 3}, {4, 4},
        {5, 5}, {6, 6}, {7, 7},
        {8, 8}, {9, 9}, {10, 10},
        {11, 11}, {12, 12}, {13, 13}
    };

    Tryte const zero{ 0 };

    std::string const start_address = "DMM";
    std::string const septavingt_chars = "MLKJIHGFEDCBA0abcdefghijklm";
    std::string const ternary_chars = "-0+";

    std::map<char, int64_t> const schar_to_val =
    { {'M', -13}, {'L', -12}, {'K', -11}, {'J', -10}, {'I', -9}, {'H', -8}, {'G', -7}, {'F', -6}, {'E', -5},
      {'D',  -4}, {'C',  -3}, {'B',  -2}, {'A',  -1}, {'0',  0}, {'a',  1}, {'b',  2}, {'c',  3}, {'d',  4},
      {'e',   5}, {'f',   6}, {'g',   7}, {'h',   8}, {'i',  9}, {'j', 10}, {'k', 11}, {'l', 12}, {'m', 13} };
    std::string const schars = "MLKJIHGFEDCBA0abcdefghijklm";

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
        "A", "B", "C", "D", "E", "F", "G", "H", "I",
        "a", "b", "c", "d", "e", "f", "g", "h", "i"
    };

    std::vector<std::string> const reg_addrs =
    {
        "[A]", "[B]", "[C]", "[D]", "[E]", "[F]", "[G]", "[H]", "[I]",
        "[a]", "[b]", "[c]", "[d]", "[e]", "[f]", "[g]", "[h]", "[i]"
    };

    std::vector<std::string> const instructions =
    {
        // memory management
        "LOAD", "load",
        "SAVE", "save",
        "BANK", "bank",
        "GANK", "gank",
        // register management
        "SET", "set",
        "SWAP", "swap",
        // stack management
        "PUSH", "push",
        "POP", "pop",
        "PEEK", "peek",
        // arithmetic ops
        "ADD", "add",
        "ADC", "adc",
        "SUB", "sub",
        "SBB", "sbb",
        "SH", "sh",
        "INC", "inc",
        "DEC", "dec",
        "FLIP", "flip",
        "ZERO", "zero",
        // logic ops
        "CMP", "cmp",
        "CPZ", "cpz",
        "AND", "and",
        "OR", "or",
        "STAR", "star",
        // control flow
        "NOP", "nop",
        "HALT", "halt",
        "SLEEP", "sleep",
        "BREAK", "break"
    };

    std::vector<std::string> const jump_instructions =
    {
        "JP", "jp",
        "PJP", "pjp",
        "JPS", "jps"
    };

    std::vector<std::string> const macros =
    {
        // equivalent to FLIP X
        "NOT", "not",
        // equivalent to SLEEP
        "WAIT", "wait",
        // CALL addr, arg1, arg2 ...
        // should expand to PUSH B, PUSH C, ... (equal to number of arguments, up to 8)
        // SET B, arg1; SET C, arg2; SET D, arg3, ...
        // JPS addr
        // POP I, POP H, ... POP C, POP B (depending on how many arguments)
        "CALL", "call",
        // equivalent to PJP, but more obviously a return
        // although will only work if the local stack has been cleared!
        "RET", "ret",
        // JPN addr - equivalent to JP addr, *, *
        "JPN", "jpn",
        // JPZ addr - equivalent to JP *, addr, *
        "JPZ", "jpz",
        // JPP addr - equivalent to JP *, *, addr
        "JPP", "jpp",
        // JPNN addr - equivalent to JP *, addr, addr
        "JPNN", "jpnn",
        // JPNZ addr - equivalent to JP addr, *, addr
        "JPNZ", "jpnz",
        // JPNP addr - equivalent to JP *, addr, addr
        "JPNP", "jpnp"
    };
}