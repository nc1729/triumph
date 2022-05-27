#include <string>

#include "constants.h"
#include "util.hpp"
#include "Token.h"

TokenType Token::infer_type(std::string const& word)
{
	if (util::is_in(constants::instructions, word))
	{
		return TokenType::INSTR;
	}

	// check if Token is an org char
	if (word == "\n")
	{
		return TokenType::NEWLINE;
	}
	else if (word == ";")
	{
		return TokenType::STATEMENT_END;
	}
	else if (word == "{")
	{
		return TokenType::BLOCK_START;
	}
	else if (word == "}")
	{
		return TokenType::BLOCK_END;
	}

	// check if Token is a register type
	if (util::is_in(constants::regs, word))
	{
		return TokenType::REG;
	}
	else if (util::is_in(constants::reg_addrs, word))
	{
		return TokenType::REG_ADDR;
	}

	return TokenType::INVALID;
}