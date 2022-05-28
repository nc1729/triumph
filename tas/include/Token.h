#pragma once

#include <string>
#include <iostream>

enum class TokenType
{
	// default
	INVALID,

	/*
	code organisation types
	*/
	// block start char {
	BLOCK_START,
	// block end char }
	BLOCK_END,
	// newline char \n - ends a statement
	NEWLINE,
	// statement end char ;
	STATEMENT_END,

	/*
	register types
	*/
	// register - any isolated char in A-I (or a-i)?
	REG,
	// indirect address - [A]
	REG_ADDR,

	/*
	basic value type
	these assemble to single Trytes
	these can be written as
	integer literals, e.g. 729
	septavingt literals - valid Trytes prepended with 0s, e.g. 0sMMM
	ternary literals - e.g. 0t+++000---
	all these are converted into "VAL" token type
	they can be valid statements on their own, to fill space or use as data
	*/
	VAL,

	// address type
	// address literal, starting with $, e.g. $MMM
	ADDR,

	// assembly mnemonic - e.g. SET, ADD, LOAD etc
	INSTR,

	// jump label, starting with !
	// a placeholder for an address in a JP statement, set at link time
	JUMP_LABEL,

	// string literal, enclosed by "" chars
	// qualify as statements in their own right - assemble to a series of ASCII trytes
	STRING,

	// a name of a code block
	// assembles to an address (which can be specified)
	NAME
};

std::ostream& operator<<(std::ostream& os, TokenType const& tokentype);

struct Token
{
	// the line number the Token was on - for error messages etc
	size_t line_number{ 0 };
	// the type of the Token
	TokenType type{ TokenType::INVALID };
	// the Token after preprocessing (removing special chars etc)
	std::string value;

	Token(std::string const& word, size_t const& line_number);

	// for debugging
	friend std::ostream& operator<<(std::ostream& os, Token const& token);
};