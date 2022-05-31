#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "Tryte.h"
#include "Token.h"

enum class StatementType
{
	INSTR,
	MACRO,
	DATA,
	JUMP,
	JUMP_LABEL
};

struct Statement
{
	StatementType type;
	std::vector<Token> tokens;
	size_t line_number;
	Tryte address = 0;
	size_t length = 0;
	std::string assembly;

	Statement(std::vector<Token> const& tokens);
	Token& operator[](size_t const i);
	Token const& operator[](size_t const i) const;
	size_t size() const;

	std::vector<Token>::iterator begin();
	std::vector<Token>::iterator end();
	std::vector<Token>::const_iterator begin() const;
	std::vector<Token>::const_iterator end() const;
};

std::ostream& operator<<(std::ostream& os, Statement const& statement);