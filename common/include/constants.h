#pragma once

#include <vector>
#include <string>
#include <map>

namespace constants
{
    std::string const start_address = "DMM";
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