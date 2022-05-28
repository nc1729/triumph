#include <string>
#include <iostream>

#include "constants.h"
#include "Tryte.h"
#include "util.hpp"
#include "Token.h"

std::ostream& operator<<(std::ostream& os, TokenType const& tokentype)
{
	if (tokentype == TokenType::INSTR)
	{
		os << "INSTR";
	}
	else if (tokentype == TokenType::BLOCK_START)
	{
		os << "BLOCK_START";
	}
	else if (tokentype == TokenType::BLOCK_END)
	{
		os << "BLOCK_END";
	}
	else if (tokentype == TokenType::NEWLINE)
	{
		os << "NEWLINE";
	}
	else if (tokentype == TokenType::STATEMENT_END)
	{
		os << "STATEMENT_END";
	}
	else if (tokentype == TokenType::REG)
	{
		os << "REG";
	}
	else if (tokentype == TokenType::REG_ADDR)
	{
		os << "REG_ADDR";
	}
	else if (tokentype == TokenType::VAL)
	{
		os << "VAL";
	}
	else if (tokentype == TokenType::ADDR)
	{
		os << "ADDR";
	}
	else if (tokentype == TokenType::JUMP_LABEL)
	{
		os << "JUMP_LABEL";
	}
	else if (tokentype == TokenType::STRING)
	{
		os << "STRING";
	}
	else if (tokentype == TokenType::NAME)
	{
		os << "STRING";
	}
	else
	{
		os << "INVALID";
	}
	return os;
}
Token::Token(std::string const& word, size_t const& line_number) :
	line_number{line_number}
{
	// check if Token is an instruction mnemonic
	if (util::is_in(constants::instructions, word))
	{
		type = TokenType::INSTR;
		value = word;
	}
	else if (word == "{")
	{
		type = TokenType::BLOCK_START;
		value = word;
	}
	else if (word == "}")
	{
		type = TokenType::BLOCK_END;
		value = word;
	}
	else if (word == "\n")
	{
		type = TokenType::NEWLINE;
		value = ""; // newlines mess with formatting
	}
	else if (word == ";")
	{
		type = TokenType::STATEMENT_END;
		value = ";";
	}
	else if (util::is_in(constants::regs, word))
	{
		type = TokenType::REG;
		value = word;
	}
	else if (util::is_in(constants::reg_addrs, word))
	{
		type = TokenType::REG_ADDR;
		// discard the [brackets]
		value = word.substr(1, 1);
	}
	else if (util::string_is_int(word))
	{
		type = TokenType::VAL;
		value = Tryte::get_str(Tryte(std::stoi(word)));
	}
	else if (util::string_is_septavingt(word))
	{
		type = TokenType::VAL;
		// remove "0s" prefix
		value = word.substr(2);
	}
	else if (util::string_is_ternary(word))
	{
		type = TokenType::VAL;
		// calculate int value of this string
		std::string trit_string = word.substr(2);
		int64_t tryte_int = 0;
		int64_t power_of_3 = 1;
		for (size_t i = 0; i < 9; i++)
		{
			int64_t sign = 0;
			// work from least significant trit to most
			if (trit_string[8 - i] == '+')
			{
				sign = 1;
			}
			else if (trit_string[8 - i] == '0')
			{
				sign = 0;
			}
			else
			{
				sign = 1;
			}
			tryte_int += (power_of_3 * sign);
			power_of_3 *= 3;
		}
		value = Tryte::get_str(Tryte(tryte_int));
	}
	else if (util::string_is_addr(word))
	{
		type = TokenType::ADDR;
		value = word.substr(1);
	}
	else if (util::string_is_jump_label(word))
	{
		type = TokenType::JUMP_LABEL;
		value = word.substr(1);
	}
	else
	{
		type = TokenType::NAME;
		// store the word for error messages
		value = word;
	}
}

std::ostream& operator<<(std::ostream& os, Token const& token)
{
	os << "[" << token.value << ", " << token.type << ", " << token.line_number << "]";
	return os;
}