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
	basic value types
	these assemble to single Trytes
	they can be valid statements on their own, to fill space or use as data
	*/
	// integer literal - e.g 729
	INT,
	// address literal, starting with $
	ADDR,
	// septavingt literal, starting with 0s - e.g. 0sMMM, 0s00a
	SV_CONST,
	// balanced ternary literal, starting with 0t - e.g. 0t+++000++-
	TN_CONST,

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

class Token
{
private:
	// from a string, infer the type of the token
	TokenType infer_type(std::string const& word);
public:
	std::string word;
	TokenType type{ TokenType::INVALID };
	size_t line_number{ 0 };

	Token(std::string const& word, size_t const& line_number) :
		word{ word }, line_number{ line_number } 
	{
		type = infer_type(word);
	};

	// for debugging
	friend std::ostream& operator<<(std::ostream& os, Token const& token);
};