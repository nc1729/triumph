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
        "HALT",
        "JP",
        "JPS",
        "PJP"
    };

    std::vector<std::string> const macros =
    {
        // equivalent to FLIP
        "NOT",
        // JPZ $X equivalent to JP *, $X, *
        "JPZ",
        // JPN $X equivalent to JP $X, *, *
        "JPN",
        // JPP $X equivalent to JP *, *, $X
        "JPP",
        // JPNZ $X equivalent to JP $X, *, $X
        "JPNZ",
        // JPNN $X equivalent to JP *, $X, $X
        "JPNN",
        // JPNP $X equivalent to JP $X, $X, *
        "JPNP"
    };
}