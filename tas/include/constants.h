#include <vector>
#include <string>

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
        "HALT"
    };

    std::vector<std::string> const jump_instructions =
    {
        "JP",
        "JPS",
        "PJP",
        "JPN",
        "JPZ",
        "JPP",
        "JPNN",
        "JPNZ",
        "JPNP"
    };

    std::vector<std::string> const macros =
    {
        // equivalent to FLIP X
        "NOT",
        // CALL addr, arg1, arg2 ...
        // should expand to PUSH B, PUSH C, ... (equal to number of arguments, up to 8)
        // SET B, arg1; SET C, arg2; SET D, arg3, ...
        // JPS addr
        // POP I, POP H, ... POP C, POP B (depending on how many arguments)
        "CALL",
        // equivalent to PJP, but more obviously a return
        // although will only work if the local stack has been cleared!
        "RET"
    };
}