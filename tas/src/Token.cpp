#include <string>
#include <iostream>

#include "common/constants.h"
#include "common/Tryte.h"
#include "common/util.h"
#include "error.h"
#include "Token.h"

bool Token::is_data_type() const
{
	return (type == TokenType::VAL || type == TokenType::ADDR || type == TokenType::STRING);
}

bool Token::is_addr_type() const
{
	return (type == TokenType::ADDR || 
			type == TokenType::REG_ADDR || 
			type == TokenType::NAME);
}

std::ostream& operator<<(std::ostream& os, TokenType const& tokentype)
{
	if (tokentype == TokenType::INSTR)
	{
		os << "INSTR";
	}
	else if (tokentype == TokenType::JUMP_INSTR)
	{
		os << "JUMP_INSTR";
	}
	else if (tokentype == TokenType::MACRO)
	{
		os << "MACRO";
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
	else if (tokentype == TokenType::ALIAS)
	{
		os << "ALIAS";
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
	else if (tokentype == TokenType::JUMP_PLACEHOLDER)
	{
		os << "JUMP_PLACEHOLDER";
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
		os << "NAME";
	}
	else
	{
		os << "INVALID";
	}
	return os;
}

Token::Token(std::string const& word, size_t const& line_number, TokenType const& tokentype) :
    word{ word }, line_number{ line_number }, type{ tokentype }
{
	if (type != TokenType::INVALID)
	{
		if (type == TokenType::STRING)
		{
			// handle special chars here
			size_t string_len = word.size();
			size_t i = 0;
			while (i < string_len)
			{
				char this_char = word[i];
				if (this_char == '\\')
				{
					// found a backslash, signifying a special char
					if (i == string_len - 1)
					{
						// found a backslash before the end of string, throw error
						throw TASError("Unexpected backslash at end of string", line_number);
					}
					char next_char = word[i+1];
					if (next_char == 'n')
					{
						// found a newline char
						value += '\n';
					}
					else if (next_char == 't')
					{
						value += '\t';
					}
					else if (next_char == 'r')
					{
						value += '\r';
					}
					else if (next_char == '\\')
					{
						value += '\\';
					}
					else if (next_char == '0')
					{
						value += '\0';
					}
					else
					{
						throw TASError("Backslash followed by unexpected control char", line_number);
					}
					i++;
				}
				else
				{
					value += this_char;
				}
				i++;
			}
		}
		else
		{
			// by default, if type given in constructor, copy value from given string
			value = word;
		}
		return;
	}

	// check if Token is an instruction mnemonic
	if (util::is_in(constants::instructions, word))
	{
		type = TokenType::INSTR;
		if (util::is_lowercase(word))
		{
			value = util::to_upper(word);
		}
		else
		{
			value = word;
		}
		
	}
	else if (util::is_in(constants::jump_instructions, word))
	{
		type = TokenType::JUMP_INSTR;
		if (util::is_lowercase(word))
		{
			value = util::to_upper(word);
		}
		else
		{
			value = word;
		}
	}
	else if (util::is_in(constants::macros, word))
	{
		type = TokenType::MACRO;
		if (util::is_lowercase(word))
		{
			value = util::to_upper(word);
		}
		else
		{
			value = word;
		}
		
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
	else if (word == "*")
	{
		type = TokenType::JUMP_PLACEHOLDER;
		value = "*";
	}
	else if (util::is_in(constants::regs, word))
	{
		type = TokenType::REG;
		// internally, regs match to lower case in trytes
		if (util::is_uppercase(word))
		{
			value = util::to_lower(word);
		}
		else
		{
			value = word;
		}
	}
	else if (util::is_in(constants::reg_addrs, word))
	{
		type = TokenType::REG_ADDR;
		// discard the [brackets]
		std::string reg = word.substr(1, 1);
		// internally, regs match to lower case in trytes
		if (util::is_uppercase(reg))
		{
			value = util::to_lower(reg);
		}
		else
		{
			value = word;
		}
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
				sign = -1;
			}
			tryte_int += (power_of_3 * sign);
			power_of_3 *= 3;
		}
		value = Tryte::get_str(Tryte(tryte_int));
	}
	else if (util::string_is_int(word))
	{
		type = TokenType::VAL;
		value = Tryte::get_str(Tryte(std::stoi(word)));
	}	
	else if (util::string_is_char(word))
	{
		type = TokenType::VAL;
		value = Tryte::get_str(Tryte(static_cast<int>(word[1])));
	}
	else if (util::string_is_control_char(word))
	{
		type = TokenType::VAL;
		if (word[2] == 'a')
		{
			value = Tryte::get_str(Tryte(static_cast<int64_t>('\a')));
		}
		else if (word[2] == 'n')
		{
			value = Tryte::get_str(Tryte(static_cast<int64_t>('\n')));
		}
		else if (word[2] == 'r')
		{
			value = Tryte::get_str(Tryte(static_cast<int64_t>('\r')));
		}
		else if (word[2] == 't')
		{
			value = Tryte::get_str(Tryte(static_cast<int64_t>('\t')));
		}
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
	else if (word == "alias" || word == "ALIAS")
	{
		type = TokenType::ALIAS;
		value = word;
	}
	else
	{
		type = TokenType::NAME;
		// append name with dollar sign, useful for finding names at link time
		value = "$" + word;
	}
}

bool Token::operator==(Token const& other)
{
	return (this->value == other.value && this->type == other.type && this->line_number == other.line_number);
}

bool Token::operator!=(Token const& other)
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, Token const& token)
{
	std::string token_contents;
	if (token.type == TokenType::STRING)
	{
		// print contents with special chars unhandled
		token_contents = token.word;
	}
	else
	{
		token_contents = token.value;
	}
	os << "[" << token_contents << ", " << token.type << ", " << token.line_number << "]";
	return os;
}
