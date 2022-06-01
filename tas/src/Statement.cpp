#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>

#include "error.h"
#include "Tryte.h"
#include "Token.h"
#include "Statement.h"

Statement::Statement(std::vector<Token> const& tokens) : tokens{ tokens }
{
	// infer type from first token in statement
	if (tokens[0].type == TokenType::INSTR)
	{
		type = StatementType::INSTR;
	}
	else if (tokens[0].type == TokenType::MACRO)
	{
		type = StatementType::MACRO;
	}
	else if (tokens[0].type == TokenType::JUMP_INSTR)
	{
		type = StatementType::JUMP;
	}
	else if (tokens[0].type == TokenType::JUMP_LABEL)
	{
		type = StatementType::JUMP_LABEL;
	}
	else if (tokens[0].is_data_type())
	{
		if (std::all_of(tokens.begin(), tokens.end(), [](Token const& t){return t.is_data_type();}))
		{
			type = StatementType::DATA;
		}
		else
		{
			throw TASError("Data statement must only contain data types (VAL, ADDR or STRING types)", 
			tokens[0].line_number);
		}
	}
	else
	{
		// invalid token to begin statement
		std::stringstream err_stream;
		err_stream << "Invalid token type " << tokens[0].type;
		err_stream << " to begin statement";
		throw TASError(err_stream.str(), tokens[0].line_number);
	}

	// infer line_number from first token
	line_number = tokens[0].line_number;
	
	// calculate length, address and assembly when assembling
}

Token& Statement::operator[](size_t const i)
{
	return tokens[i];
}

Token const& Statement::operator[](size_t const i) const
{
	return tokens[i];
}

size_t Statement::size() const
{
	return tokens.size();
}

std::vector<Token>::iterator Statement::begin()
{
	return tokens.begin();
}

std::vector<Token>::iterator Statement::end()
{
	return tokens.end();
}

std::vector<Token>::const_iterator Statement::begin() const
{
	return tokens.cbegin();
}

std::vector<Token>::const_iterator Statement::end() const
{
	return tokens.cend();
}

std::ostream& operator<<(std::ostream& os, Statement const& statement)
{
	for (Token const& token : statement)
	{
		if (token.type == TokenType::STRING)
		{
			os << '\"' << token.value << '\"' << ' ';
		}
		else
		{
			os << token.value << ' ';
		}
	}
	return os;
}