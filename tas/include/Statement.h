#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "common/Tryte.h"
#include "Token.h"

enum class StatementType
{
	INSTR,
	MACRO,
	DATA,
	JUMP,
	JUMP_LABEL
};

std::ostream& operator<<(std::ostream& os, StatementType const& type);

class Statement
{
public:
	StatementType type;
	std::vector<Token> tokens;
	size_t line_number;
	std::vector<std::string> assembled_trytes;

	Statement(std::vector<Token> const& tokens);
	// allow construction of statement from single Token
	Statement(Token const& token) : Statement(std::vector<Token>{token}) {};
	Token& operator[](size_t const i);
	Token const& operator[](size_t const i) const;
	size_t size() const;
	size_t tryte_length() const;

	std::vector<Token>::iterator begin();
	std::vector<Token>::iterator end();
	std::vector<Token>::const_iterator begin() const;
	std::vector<Token>::const_iterator end() const;
};

std::ostream& operator<<(std::ostream& os, Statement const& statement);