#include <vector>
#include <string>
#include <iostream>

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
	else
	{
		type = StatementType::DATA;
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

std::ostream& operator<<(std::ostream& os, Statement const& statement)
{
	for (Token const& token : statement.tokens)
	{
		if (token.type == TokenType::STRING)
		{
			os << '\"' << token.word << '\"' << ' ';
		}
		else
		{
			os << token.word << ' ';
		}
	}
	return os;
}